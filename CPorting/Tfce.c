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
			toReturn[i] = label;
			(*num_clusters)++;
			while(!isEmpty(q)){
				actual_index = Front(q);
				Dequeue(q);
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
							toCheck = linearIndexFromCoordinate(x+j, y+k, z+h, dim_x, dim_y);	
							if (toReturn[toCheck] == 1) {
								Enqueue(q, toCheck);
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
	free(q);
	return toReturn;
}

//computes the tfce score of a 3D statistic map(dim_x*dim_y*dim_z)
float * tfce_score(float * map, int dim_x, int dim_y, int dim_z, float E, float H, float dh, int logging){
	int n = dim_x * dim_y * dim_z;
	float minData = 0; float maxData = 0; float rangeData = 0;
	float * posData; float * negData;
	float precision, increment;
	float pos_increment, neg_increment;
	float h;
	int i,j;
	int * indexPosData; int * indexNegData; int * indexMatchingData;
	float * clustered_map;
	int num_clusters;
	int numOfElementsMatching;
	float * toReturn = fill0(n);
	float minPos = 0, maxPos = 0;
	float minNeg = 0, maxNeg = 0;
	float steps = 0;
	FILE* log;
	
	if(logging){
		log = fopen("clustered.txt", "w");
	}

	findMinMax(map, n, &minData, &maxData, &rangeData);
	precision = rangeData/dh;

	if (precision > 200) {
		increment = rangeData/200;
	} else{
		increment = rangeData/precision;	
	}
	if(minData >= 0){
		for (h = minData; h < maxData; h += increment) {
			indexMatchingData = getBinaryVector(map, n, moreThan, h, &numOfElementsMatching);
			if(logging){
				for(i = 0; i < n; i++){
					fprintf(log, "%d ", indexMatchingData[i]);
				}
				fprintf(log, "\n58\n40\n46\n");
				fclose(log);
			}
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
		indexPosData = getBinaryVector(map, n, moreThan, 0, &numOfElementsMatching);
		posData = fromBinaryToRealVector(map, n, indexPosData);
		findMinMax(posData, n, &minPos, &maxPos, &rangeData);
		steps = roundf((maxPos - minPos)/(increment));
		pos_increment = (maxPos - minPos)/(steps);
		for (h = minPos; h < maxPos; h += pos_increment) {
			indexMatchingData = getBinaryVector(posData, n, moreThan, h, &numOfElementsMatching);
			num_clusters = 0;
			clustered_map = find_clusters_3D(indexMatchingData, dim_x, dim_y, dim_z, n, &num_clusters);
			if(logging){
				for(i = 0; i < n; i++){
					fprintf(log, "%d\n", (int)clustered_map[i]);
				}
				fprintf(log, "58\n40\n46\n");
				fclose(log);
			}
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

		free(posData);
		free(indexPosData);

		indexNegData = getBinaryVector(map, n, lessThan, 0, &numOfElementsMatching);
		negData = fromBinaryToRealVector(map, n, indexNegData);
		abs_vector(negData,n);
		findMinMax(negData, n, &minNeg, &maxNeg, &rangeData);
		steps = roundf((maxNeg - minNeg)/increment);
		neg_increment = (maxNeg - minNeg)/(steps);
		for (h = minNeg; h < maxNeg; h += neg_increment) {
			indexMatchingData = getBinaryVector(negData, n, moreThan, h, &numOfElementsMatching);
			num_clusters = 0;
			clustered_map = find_clusters_3D(indexMatchingData, dim_x, dim_y, dim_z, n, &num_clusters);
			free(indexMatchingData);
			for (i = 1; i <= num_clusters; ++i) {
				numOfElementsMatching = 0;
				indexMatchingData = getBinaryVector(clustered_map, n, equalTo, i, &numOfElementsMatching);
				for (j = 0; j < n; ++j) {
					if (clustered_map[j] == i) {
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
		free(negData);
		free(indexNegData);
	}
	return toReturn;
}
