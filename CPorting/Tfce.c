#include "Tfce.h"
#include "Queue.h"
#include "Utilities.h"
#include <math.h>
#include <stdlib.h>


double * find_clusters_3D(int * binaryVector, int dim_x, int dim_y, int dim_z, int n, int * num_clusters){
	int label = 2;
	int x, y, z;
	int toCheck;
	int i, j, k, h;
	int actual_index;
	QueuePtr q = newQueue();
	(*num_clusters) = 0;
	double * toReturn = copyAndConvertIntVector(binaryVector, n);
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

void printToFile(FILE* log, double * clustered_map, int n, int logging){
	int i;
	if(logging){
		for(i=0; i < n; i++){
			fprintf(log, "%f \n", clustered_map[i]);
		}
		fclose(log);
	}
}


void computeTfceIteration(double h, double * map, int n, int dim_x, int dim_y, int dim_z, double E, double H, double dh, double * toReturn){
	int i, numOfElementsMatching, j;
	int * indexMatchingData = getBinaryVector(map, n, moreThan, h, &numOfElementsMatching);
	int num_clusters = 0;
	double * clustered_map = find_clusters_3D(indexMatchingData, dim_x, dim_y, dim_z, n, &num_clusters);
	free(indexMatchingData);
	for (i = 1; i <= num_clusters; ++i) {
		numOfElementsMatching = 0;	
		//getBinaryVector is called just for obtain numOfElementsMatching
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
	FILE* log;
	
	if(logging)
		log = fopen("cluNegC.txt", "w");
	

	findMinMax(map, n, &minData, &maxData, &rangeData);
	precision = rangeData/dh;

	if (precision > 200) {
		increment = rangeData/200;
	} else{
		increment = rangeData/precision;	
	}

	if(minData >= 0){
		for (h = minData; h < maxData; h += increment) {
			computeTfceIteration(h, map, n, dim_x, dim_y, dim_z, E, H, dh, toReturn);
		}
	}
	else{
		indexPosData = getBinaryVector(map, n, moreThan, 0, &numOfElementsMatching);
		posData = fromBinaryToRealVector(map, n, indexPosData);
		findMinMax(posData, n, &minPos, &maxPos, &rangeData);
		steps = round((maxPos - minPos)/(increment));
		pos_increment = (maxPos - minPos)/(steps);
		for (h = minPos; h < maxPos; h += pos_increment) {
			computeTfceIteration(h, posData, n, dim_x, dim_y, dim_z, E, H, dh, toReturn);
		}

		free(posData);
		free(indexPosData);

		indexNegData = getBinaryVector(map, n, lessThan, 0, &numOfElementsMatching);
		negData = fromBinaryToRealVector(map, n, indexNegData);
		abs_vector(negData,n);
		findMinMax(negData, n, &minNeg, &maxNeg, &rangeData);
		steps = round((maxNeg - minNeg)/increment);
		neg_increment = (maxNeg - minNeg)/(steps);
		for (h = minNeg; h < maxNeg; h += neg_increment) {
			computeTfceIteration(h, negData, n, dim_x, dim_y, dim_z, E, H, dh, toReturn);
		}
		free(negData);
		free(indexNegData);
	}
	return toReturn;
}
