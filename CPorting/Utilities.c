#include "Utilities.h"
#include <math.h>
#include <stdlib.h>

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
	(*range) = abs(max - min);
}

//performs confrontation between and b given a single char operation
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

//Gets a binary vector in matlab style, given a certain operation
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

//Applies a single char operation on every element of the vector
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
