#include "Tfce.h"
#include "Queue.h"
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
	QueuePtr q = newQueue();
	(*num_clusters) = 0;
	int * toReturn = (int *) calloc(sizeof(int), n);
	for (i = 0; i < n; ++i) 
		toReturn[i] = binaryVector[i];
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

void printToFile(const char * filename, void * map, int n, int isInteger){
	int i;
	double * clustered_map;
	int * other_map =  (int *) map;
	clustered_map = (double *) map;
	FILE * log = fopen(filename, "w");
	if(!isInteger){
		for(i=0; i < n; i++){
			fprintf(log, "%lf \n", clustered_map[i]);
		}
		fclose(log);
	}
	else{
		for(i=0; i < n; i++){
			fprintf(log, "%d \n", other_map[i]);
		}
		fclose(log);
	}
}


void computeTfceIteration(double h, double * map, int n, int dim_x, int dim_y, int dim_z, double E, double H, double dh, double * toReturn, int isPositive){
	int i, numOfElementsMatching, j;
	int * indexMatchingData = getBinaryVector(map, n, moreThan, h, &numOfElementsMatching);
	int num_clusters = 0;
	char string_h[10]; 
	char default_path [50];
	if(isPositive)
		sprintf(default_path, "pos/cluster_pos_");
	else
		sprintf(default_path, "neg/cluster_neg_");
	sprintf(string_h, "%lf", h);
	char * concatenated_string = strcat(default_path, string_h);
	int * clustered_map = find_clusters_3D(indexMatchingData, dim_x, dim_y, dim_z, n, &num_clusters);
	int * extent_map = (int *) calloc(sizeof(int), n);
	for (j = 0; j < n; ++j){
		extent_map[j] = 0;
	}
	double * clustered_map_double;
	free(indexMatchingData);
	for (i = 1; i <= num_clusters; ++i) {
		numOfElementsMatching = 0;	
		//getBinaryVector is called just for obtain numOfElementsMatching
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
        printToFile(concatenated_string, clustered_map, n, 1);
	clustered_map_double = copyAndConvertIntVector(extent_map, n);
	apply_function(clustered_map_double, n, elevate, E);
	apply_function(clustered_map_double, n, multiply, pow(h, H));
	apply_function(clustered_map_double, n, multiply, dh);
	for (i = 0; i < n; ++i) {
		toReturn[i] += clustered_map_double[i];
	}
	free(clustered_map_double);
	free(clustered_map);
	free(extent_map);
}


//computes the tfce score of a 3D statistic map(dim_x*dim_y*dim_z)
double * tfce_score(double * map, int dim_x, int dim_y, int dim_z, double E, double H, double dh, int logging){
	int n = dim_x * dim_y * dim_z;
	double minData = 0; double maxData = 0; double rangeData = 0;
	double * posData; double * negData;
	double precision, increment;
	double pos_increment, neg_increment;
	double h;
	int i,j;
	int * indexPosData; int * indexNegData; int * indexMatchingData;
	double * clustered_map;
	int num_clusters;
	int numOfElementsMatching;
	double * toReturn = fill0(n);
	double minPos = 0; double maxPos = 0;
	double minNeg = 0; double maxNeg = 0;
	double steps = 0;	

	findMinMax(map, n, &minData, &maxData, &rangeData);
	precision = rangeData/dh;

	if (precision > 200) {
		increment = rangeData/200;
	} else{
		increment = rangeData/precision;	
	}

	if(minData >= 0){
		for (h = minData; h <= maxData; h += increment) {
			computeTfceIteration(h, map, n, dim_x, dim_y, dim_z, E, H, dh, toReturn, 1);
		}
	}
	else{
		indexPosData = getBinaryVector(map, n, moreThan, 0, &numOfElementsMatching);
		//printToFile("index_pos_data.txt", indexPosData, n, 1);
		posData = fromBinaryToRealVector(map, n, indexPosData);
		//printToFile("pos_data.txt", posData, n, 0);
		findMinMax(posData, n, &minPos, &maxPos, &rangeData);
		steps = round((maxPos - minPos)/(increment));
		pos_increment = (maxPos - minPos)/(steps);
		for (h = minPos; h <= maxPos; h += pos_increment) {
			computeTfceIteration(h, posData, n, dim_x, dim_y, dim_z, E, H, dh, toReturn, 1);
		}
		free(posData);
		free(indexPosData);
		//printToFile("tfce_score_positives.txt", toReturn, n, 0);
		indexNegData = getBinaryVector(map, n, lessThan, 0, &numOfElementsMatching);
		//printToFile("index_neg_data.txt", indexNegData, n, 1);
		negData = fromBinaryToRealVector(map, n, indexNegData);
		abs_vector(negData,n);
		//printToFile("neg_data.txt", negData, n, 0);
		findMinMax(negData, n, &minNeg, &maxNeg, &rangeData);
		steps = round((maxNeg - minNeg)/increment);
		neg_increment = (maxNeg - minNeg)/(steps);
		for (h = minNeg; h <= maxNeg; h += neg_increment) {
			computeTfceIteration(h, negData, n, dim_x, dim_y, dim_z, E, H, dh, toReturn, 0);
		}
		free(negData);
		free(indexNegData);
	}
	return toReturn;
}
