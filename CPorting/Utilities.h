//
// Created by Luigi Giugliano on 08/02/16.
//

#ifndef CPORTING_UTILITIES_H
#define CPORTING_UTILITIES_H

//difference between min and max is the range
int findMinMax(float * map, float * min, float * max);
int * getBinaryVector(float * map, const char operation, float value);
int getNeighbours(int i);

#endif //CPORTING_UTILITIES_H
