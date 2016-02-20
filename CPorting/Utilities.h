#ifndef UTILITIES_H
#define UTILITIES_H
#include <float.h>
#include <stdio.h>

//Finds minimum, maximum and range of a float vector  
void findMinMax(double *map, int n, double *min, double *max, double * range);

//performs confrontation between and b given a single char operation
int confront(double a, double b, char operation);

//Gets a binary vector in matlab style, given a certain operation
int * getBinaryVector(double * map, int n, int (*confront)(double, double), double value, int * numOfElementsMatching);

//Given a binary vector and a real vector, fills a new vector in the indices where binary == 1
double * fromBinaryToRealVector(double * map, int n, int * binaryVector);

//creates a n-element vector of float filled with 0
double * fill0(int n);

//Applies a single char operation on every element of the vector
void apply_function(double * vector, int n, double (* operation) (double a, double b), double argument);

//converts 3D to 1D index
int linearIndexFromCoordinate(int x, int y, int z, int max_x, int max_y);
//converts 1D to 3D index
void coordinatesFromLinearIndex(int index, int max_x, int max_y, int * x, int * y, int * z);

//copies an int vector into a float vector
double * copyAndConvertIntVector(int * vector, int n);
#endif //UTILITIES_H

int lessThan(double a, double b);

int equalTo(double a, double b);

int moreThan(double a, double b);

double multiply(double a, double b);

double elevate(double a, double b);

void abs_vector(double * vector, int n);

double * readMatFromFile(FILE * fp, int * size, int * x, int * y, int * z);

void * shuffle(double *array, int n);
