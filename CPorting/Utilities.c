#include "Utilities.h"
#include "Queue.h"
#include <math.h>
#include <stdlib.h>
#include <float.h>

void findMinMax(float *map, int n, float *min, float *max, float * range) {
	(*min) = FLT_MAX;
	(*max) = FLT_MIN;
	for (int i = 0; i < n; ++i) {
		if (map[i] < (*min))
			(*min) = map[i];
		if (map[i] > (*max))
			(*max) = map[i];
	}
	(*range) = abs(max - min);
}

int confront(float a, float b, char operation){
	switch (operation) {
		case '<':
			return a<b;
		case '>':
			return a>b;
		case '=':
			return a==b;
	}
}

int * getBinaryVector(float * map, int n, const char operation, float value, int * numOfElementsMatching){
	int * binaryVector = (int *) calloc(sizeof(int), n);
	int i;

	for (i = 0; i < n; ++i) {
		if(confront(map[i],value, operation)){
			binaryVector[i] = 1;
			(*numOfElementsMatching)++;
		}
		else
			binaryVector[i] = 0;
	}
	return binaryVector;
}

float * fromBinaryToRealVector(float * map, int n, int * binaryVector){
	float * toReturn = (float *) calloc(sizeof(float), n);
	int i;

	for (i = 0; i < n; ++i) {
		if(binaryVector[i] == 1){
			toReturn[i] = map[i];
		}else{
			toReturn[i] = 0;
		}	
	}
	return toReturn;
}

float * fill0(int n){
	float * toReturn = (float *) calloc(sizeof(float), n);
	int i;
	for (i = 0; i < n; ++i) {
		toReturn[i] = 0;
	}
	return toReturn;
}

void apply_function(float * vector, int n, const char op, float argument){
	int i;
	switch (op) {
		case '^': 
			for (i = 0; i < n; ++i) {
				vector[i] = pow(vector[i], argument);
			}	
			break;
		case '*':
			for (i = 0; i < n; ++i) {
				vector[i] *= argument;
			}
			break;
	}
}

int linearIndexFromCoordinate(int x, int y, int z, int max_x, int max_y){
	return(z*max_x*max_y + y*max_x + x);
}

void coordinatesFromLinearIndex(int index, int max_x, int max_y, int * x, int * y, int * z){
	(*x) =  index % (max_x);
	index /= (max_x);
	(*y) = index % (max_y);
	index /= (max_y);
	(*z) = index;
}

float * copyAndConvertIntVector(int * vector, int n){
	float * toReturn = (float *) calloc(sizeof(float), n);
	int i;
	for (i = 0; i < n; ++i) {
		toReturn[i] = vector[i];
	}
	return toReturn;
}

float * find_clusters_3D(int * binaryVector, int dim_x, int dim_y, int dim_z, int n, int * num_clusters){
	int label = 2;
	int x, y, z;
	int toCheck;
	int i, j, k, h;
	int actual_index;
	QueuePtr q = newQueue();
	float * toReturn = copyAndConvertIntVector(binaryVector, n);
	free(binaryVector);
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
							if (x+j < 0 || x+j > dim_x) {
								break;	
							}
							if (y+k < 0 || y+k > dim_y) {
								break;	
							}
							if (z+h < 0 || z+h > dim_z) {
								break;	
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

float * tfce_score(float * map, int dim_x, int dim_y, int dim_z, float E, float H, float dh){
	int n = dim_x * dim_y * dim_z;
	float minData, maxData, rangeData;
	float * posData, negData;
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
	if(minData > 0){
		for (h = minData; i < maxData; i += increment) {
			indexMatchingData = getBinaryVector(map, n, '>', h, &numOfElementsMatching);
			num_clusters = 0;
			clustered_map = find_clusters_3D(indexMatchingData, dim_x, dim_y, dim_z, n, &num_clusters);
			free(indexMatchingData);
			for (i = 1; i <= num_clusters; ++i) {
				numOfElementsMatching = 0;	
				indexMatchingData = getBinaryVector(clustered_map, n, '=', i, &numOfElementsMatching);
				for (j = 0; j < n; ++j) {
					if(clustered_map[j] == i){
						clustered_map[j] = numOfElementsMatching;
					}	
				}
				free(indexMatchingData);
			}
			apply_function(clustered_map, n, '^', E);
			apply_function(clustered_map, n, '*', pow(h, H));
			apply_function(clustered_map, n, '*', dh);
			for (i = 0; i < n; ++i) {
				toReturn[i] += clustered_map[i];
			}
			free(clustered_map);
		}
	}
	else{
		indexPosData = getBinaryVector(map, n, '<', 0, &numOfElementsMatching);
		indexNegData = getBinaryVector(map, n, '>', 0, &numOfElementsMatching);
		// posData = fromBinaryToRealVector(map, n, indexPosData);
		// negData = fromBinaryToRealVector(map, n, indexNegData);
		//------------------------TODO
		free(indexPosData);
		free(indexNegData);
	}
	return toReturn;
}
