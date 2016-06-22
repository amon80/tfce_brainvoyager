#include "Tfce.h"
#include <queue>
#include "Utilities.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


int * find_clusters_3D(int * binaryVector, int dim_x, int dim_y, int dim_z, int n, int * num_clusters){
	int label = 2;
	int x, y, z;
	int toCheck;
	int i, j, k, h;
	int actual_index;
	std::queue<int> q;
	(*num_clusters) = 0;
	int * toReturn = new int[n];
	for (i = 0; i < n; ++i) 
		toReturn[i] = binaryVector[i];
	for (i = 0; i < n; ++i) {
		if (toReturn[i] == 1) {
			q.push(i);
			toReturn[i] = label;
			(*num_clusters)++;
			while(!q.empty()){
				actual_index = q.front();
				q.pop();
				x = 0; y = 0; z = 0;
				coordinatesFromLinearIndex(actual_index, dim_x, dim_y, &x, &y, &z);			
				for (j = -1; j <= 1; ++j) {
					if (x+j < 0)
						continue;	
					if (x+j >= dim_x)
						continue;
					for (k = -1; k <= 1; ++k) {
						if (y+k < 0)
							continue;	
						if (y+k >= dim_y)
							continue;
						for (h = -1; h <= 1; ++h) {
							if (z+h < 0) 
								continue;	
							if (z+h >= dim_z)
								continue;
							toCheck = linearIndexFromCoordinate(x+j, y+k, z+h, dim_x, dim_y);	
							if (toReturn[toCheck] == 1) {
								q.push(toCheck);
								toReturn[toCheck] = label;
							}
						}	
					}	
				}
			}
			label++;
		}
	}
	for (i = 0; i < n; ++i) {
		if (toReturn[i] != 0) 
			toReturn[i]--;
	}
	return toReturn;
}


void computeTfceIteration(float h, float * map, int n, int dim_x, int dim_y, int dim_z, float E, float H, float dh, float * toReturn){
	int i = 0, numOfElementsMatching = 0, j = 0;
	int * indexMatchingData = getBinaryVector(map, n, moreThan, h, &numOfElementsMatching);
	int num_clusters = 0;
	char string_h[10]; 
	char default_path [50];
	float * clustered_map_float;
	char * concatenated_string;
	int * clustered_map;
	int * extent_map;
	clustered_map = find_clusters_3D(indexMatchingData, dim_x, dim_y, dim_z, n, &num_clusters);
	extent_map = new int[n];
	for (j = 0; j < n; ++j){
		extent_map[j] = 0;
	}
	delete [] indexMatchingData;
	for (i = 1; i <= num_clusters; ++i) {
		numOfElementsMatching = 0;	
		for (j = 0; j < n; ++j){
			if(clustered_map[j] == i){
				numOfElementsMatching++;
			}
		}
		for (j = 0; j < n; ++j) {
			if(clustered_map[j] == i){
				extent_map[j] = numOfElementsMatching;
			}	
		}
	}
	clustered_map_float = copyAndConvertIntVector(extent_map, n);
	apply_function(clustered_map_float, n, elevate, E);
	apply_function(clustered_map_float, n, multiply, pow(h, H));
	apply_function(clustered_map_float, n, multiply, dh);
	for (i = 0; i < n; ++i) {
#pragma omp atomic
			toReturn[i] += (clustered_map_float[i]);
	}
	delete[] clustered_map_float;
	delete[] clustered_map;
	delete[] extent_map;

}



//computes the tfce score of a 3D statistic map(dim_x*dim_y*dim_z)
float * tfce_score(float * map, int dim_x, int dim_y, int dim_z, float E, float H, float dh){
	int n = dim_x * dim_y * dim_z;
	float minData = 0; float maxData = 0; float rangeData = 0;
	float * posData; float * negData;
	float precision, increment;
	float h;
	int i,j;
	int * indexPosData; int * indexNegData; int * indexMatchingData;
	float * clustered_map;
	int num_clusters;
	int numOfElementsMatching;
	float * toReturn = fill0(n);
	float minPos = 0; float maxPos = 0;
	int steps = 0;

	findMinMax(map, n, &minData, &maxData, &rangeData);
	precision = rangeData/dh;

	if (precision > 200) {
		increment = rangeData/200;
	} else{
		increment = rangeData/precision;	
	}

	steps = ceil(rangeData / increment);
#pragma omp parallel for
	for (i = 0; i < steps; i++) {
		computeTfceIteration(minData + i*increment, map, n, dim_x, dim_y, dim_z, E, H, increment, toReturn);
	}
	
	return toReturn;
}
