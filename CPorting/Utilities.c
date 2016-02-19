#include "Utilities.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Finds minimum, maximum and range of a float vector  
void findMinMax(double *map, int n, double *min, double *max, double * range) {
	(*min) = FLT_MAX;
	(*max) = FLT_MIN;
	for (int i = 0; i < n; ++i) {
		if (map[i] < (*min))
			(*min) = map[i];
		if (map[i] > (*max))
			(*max) = map[i];
	}
	(*range) = (*max - *min);
}

//performs confrontation between and b given a single char operation
int lessThan(double a, double b){
        return a < b;
}

int equalTo(double a, double b){
        return a == b;
}

int moreThan(double a, double b){
        return a > b;
}

//Gets a binary vector in matlab style, given a certain operation
int * getBinaryVector(double * map, int n, int (*confront)(double, double), double value, int * numOfElementsMatching){
	int * binaryVector = (int *) calloc(sizeof(int), n);
	int i;

	for (i = 0; i < n; ++i) {
		if(confront(map[i],value)){
			binaryVector[i] = 1;
			(*numOfElementsMatching)++;
		}
		else
			binaryVector[i] = 0;
	}
	return binaryVector;
}

//Given a binary vector and a real vector, fills a new vector in the indices where binary == 1
double * fromBinaryToRealVector(double * map, int n, int * binaryVector){
	double * toReturn = (double *) calloc(sizeof(double), n);
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

//creates a n-element vector of doubles filled with 0
double * fill0(int n){
	double * toReturn = (double *) calloc(sizeof(double), n);
	int i;
	for (i = 0; i < n; ++i) {
		toReturn[i] = 0;
	}
	return toReturn;
}

double multiply(double a, double b){
	return a * b;
}

double elevate(double a, double b){
	return pow(a,b);
}

void apply_function(double * vector, int n, double (* operation) (double a, double b), double argument){
	int i;
	for (i = 0; i < n; ++i) {
	    vector[i] = operation(vector[i], argument);
	}
}

void abs_vector(double * vector, int n){
	int i;
	for (i = 0; i < n; ++i) {
		if(vector[i] < 0)
			vector[i] = -vector[i];
	}
}

//converts 3D to 1D index
int linearIndexFromCoordinate(int x, int y, int z, int max_x, int max_y){
	return(z*max_x*max_y + y*max_x + x);
}

//converts 1D to 3D index
void coordinatesFromLinearIndex(int index, int max_x, int max_y, int * x, int * y, int * z){
	(*x) =  index % (max_x);
	index /= (max_x);
	(*y) = index % (max_y);
	index /= (max_y);
	(*z) = index;
}

//copies an int vector into a float vectorS
double * copyAndConvertIntVector(int * vector, int n){
	double * toReturn = (double *) calloc(sizeof(double), n);
	int i;
	for (i = 0; i < n; ++i) {
		toReturn[i] = (double)vector[i];
	}
	return toReturn;
}

double * readMatFromFile(FILE *fp, int * size, int * x, int * y, int * z){
	char * line = NULL;
	char * oldline = NULL;
	char *token;
	double *mat;
	size_t len = 0;
	ssize_t read;

	if (fp == NULL)
		exit(EXIT_FAILURE);

	int numline =0;

	while ((read = getline(&line, &len, fp)) != -1) {

		if (numline==0) {
			//printf("%zd - %zu \n ", len, read);

			//removing newline
			line[strcspn(line, "\n")] = 0;
			oldline = (char *) calloc(read + 1, sizeof(char));
			strcpy(oldline, line);
			int num_of_val = 0;
			for (token = strtok(line, " "); token != NULL; token = strtok(NULL, " ")) {
				num_of_val++;
			}
			//printf("Num of val %d\n", num_of_val);
			mat  = (double *) calloc(num_of_val, sizeof(double));

			num_of_val = 0;
			
			for (token = strtok(oldline, " "); token != NULL; token = strtok(NULL, " ")) {
				mat[num_of_val] = (double)atof(token);
				//printf("%f\n",mat[num_of_val]);
				num_of_val++;
			}

			line = NULL;
			*size = num_of_val;
		}

		if(numline==1) {
			line[strcspn(line, "\n")] = 0;
			 *x = atoi(line);
		}
		if(numline==2){
			line[strcspn(line, "\n")] = 0;
			*y=atoi(line);
		}
		if(numline==3) {
			line[strcspn(line, "\n")] = 0;
			*z = atoi(line);
		}
		numline++;
	}
	//printf("Read Complete\n");
	return  mat;
}
