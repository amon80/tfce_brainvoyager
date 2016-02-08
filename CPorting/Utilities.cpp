//
// Created by Luigi Giugliano on 08/02/16.
//

#include "Utilities.h"

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
