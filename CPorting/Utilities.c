#include "Utilities.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 200000

//Finds minimum, maximum and range of a float vector  
void findMinMax(float *map, int n, float *min, float *max, float * range) {
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
int lessThan(float a, float b){
        return a < b;
}

int equalTo(float a, float b){
        return a == b;
}

int moreThan(float a, float b){
        return a > b;
}

//Gets a binary vector in matlab style, given a certain operation
int * getBinaryVector(float * map, int n, int (*confront)(float, float), float value, int * numOfElementsMatching){
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

//creates a n-element vector of float filled with 0
float * fill0(int n){
	float * toReturn = (float *) calloc(sizeof(float), n);
	int i;
	for (i = 0; i < n; ++i) {
		toReturn[i] = 0;
	}
	return toReturn;
}

float multiply(float a, float b){
	return a * b;
}

float elevate(float a, float b){
	return pow(a,b);
}

//Applies a single char operation on every element of the vector
void apply_function(float * vector, int n, float (* operation) (float a, float b), float argument){
	int i;
	for (i = 0; i < n; ++i) {
	    vector[i] = operation(vector[i], argument);
	}
}

void abs_vector(float * vector, int n){
	int i;
	for (i = 0; i < n; ++i) {
		vector[i] = fabsf(vector[i]);
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
float * copyAndConvertIntVector(int * vector, int n){
	float * toReturn = (float *) calloc(sizeof(float), n);
	int i;
	for (i = 0; i < n; ++i) {
		toReturn[i] = vector[i];
	}
	return toReturn;
}

float * readMatFromFile(FILE *fp, int * size, int * x, int * y, int * z){

	char * line = NULL;
	char * oldline = NULL;
	float *mat;
	size_t len = 0;
	ssize_t read;

	if (fp == NULL)
		exit(EXIT_FAILURE);

	int numline =0;

	while ((read = getline(&line, &len, fp)) != -1) {

		if (numline==0) {
			printf("%zd - %zu \n ", len, read);

			char *token;
			oldline = (char *) calloc(read + 1, sizeof(char));
			strcpy(oldline, line);
			int num_of_val = 0;
			for (token = strtok(line, " "); token != NULL; token = strtok(NULL, " ")) {
				num_of_val++;
			}
			//printf("Num of val %d\n", num_of_val);
			mat  = (float *) calloc(num_of_val, sizeof(float));

			num_of_val = 0;
			for (token = strtok(oldline, " "); token != NULL; token = strtok(NULL, " ")) {
				mat[num_of_val] = atof(token);
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
	printf("Read Complete\n");
	fclose(fp);
	return  mat;

	fclose(fp);
	return NULL;




}
