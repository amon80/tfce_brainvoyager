#include "Tfce.h"
#include "Queue.h"
#include "Utilities.h"
#include <math.h>
#include <stdlib.h>


float * find_clusters_3D(int * binaryVector, int dim_x, int dim_y, int dim_z, int n, int * num_clusters){
	int label = 2;
	int x, y, z;
	int toCheck;
	int i, j, k, h;
	int actual_index;
	QueuePtr q = newQueue();
	float * toReturn = copyAndConvertIntVector(binaryVector, n);
	//free(binaryVector); not necessary, we'll do it outside
	for (i = 0; i < n; ++i) {
		if (toReturn[i] == 1) {
			Enqueue(q, i);
			(*num_clusters)++;
			while(!isEmpty(q)){
				actual_index = Front(q);
				Dequeue(q);
				toReturn[actual_index] = label;
				x = 0; y = 0; z = 0;
				coordinatesFromLinearIndex(actual_index, dim_x, dim_y, &x, &y, &z);			
				for (j = -1; j <= 1; ++j) {
					for (k = -1; k <= 1; ++k) {
						for (h = -1; h <= 1; ++h) {
							if (x+j < 0 || x+j >= dim_x) {
								continue;	
							}
							if (y+k < 0 || y+k >= dim_y) {
								continue;	
							}
							if (z+h < 0 || z+h >= dim_z) {
								continue;	
							}
							toCheck = linearIndexFromCoordinate(x+j, y+k, z+h, dim_y, dim_z);	
							if (toReturn[toCheck] == 1) {
								Enqueue(q, toCheck);
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

//computes the tfce score of a 3D statistic map(dim_x*dim_y*dim_z)
float * tfce_score(float * map, int dim_x, int dim_y, int dim_z, float E, float H, float dh){
	int n = dim_x * dim_y * dim_z;
	float minData, maxData, rangeData;
	float * posData; float * negData;
	float precision, increment;
	float h;
	int i,j;
	int * indexPosData; int * indexNegData; int * indexMatchingData;
	float * clustered_map;
	int num_clusters;
	int numOfElementsMatching;
	float * toReturn = fill0(n);

	findMinMax(map, n, &minData, &maxData, &rangeData);
	precision = rangeData/dh;

	if (precision > 200) {
		increment = rangeData/200;
	} else{
		increment = rangeData/precision;	
	}
	if(minData >= 0){
		for (h = minData; i < maxData; i += increment) {
			indexMatchingData = getBinaryVector(map, n, lessThan, h, &numOfElementsMatching);
			num_clusters = 0;
			clustered_map = find_clusters_3D(indexMatchingData, dim_x, dim_y, dim_z, n, &num_clusters);
			free(indexMatchingData);
			for (i = 1; i <= num_clusters; ++i) {
				numOfElementsMatching = 0;	
				indexMatchingData = getBinaryVector(clustered_map, n, equalTo, i, &numOfElementsMatching);
				for (j = 0; j < n; ++j) {
					if(clustered_map[j] == i){
						clustered_map[j] = numOfElementsMatching;
					}	
				}
				free(indexMatchingData);
			}
			apply_function(clustered_map, n, elevate, E);
			apply_function(clustered_map, n, multiply, pow(h, H));
			apply_function(clustered_map, n, multiply, dh);
			for (i = 0; i < n; ++i) {
				toReturn[i] += clustered_map[i];
			}
			free(clustered_map);
		}
	}
	else{
		indexPosData = getBinaryVector(map, n, lessThan, 0, &numOfElementsMatching);
		indexNegData = getBinaryVector(map, n, moreThan, 0, &numOfElementsMatching);
		// posData = fromBinaryToRealVector(map, n, indexPosData);
		// negData = fromBinaryToRealVector(map, n, indexNegData);
		//------------------------TODO
		free(indexPosData);
		free(indexNegData);
	}
	return toReturn;
}
