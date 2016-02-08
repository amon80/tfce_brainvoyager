#ifndef CPORTING_UTILITIES_H
#define CPORTING_UTILITIES_H
float * tfce_score(float * map, int dim_x, int dim_y, int dim_z, float E, float H, float dh);
int linearIndexFromCoordinate(int x, int y, int z, int max_x, int max_y);
void coordinatesFromLinearIndex(int index, int max_x, int max_y, int * x, int * y, int * z);
#endif //CPORTING_UTILITIES_H
