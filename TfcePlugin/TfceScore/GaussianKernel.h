#ifndef GAUSSIANKERNEL_H
#define GAUSSIANKERNEL_H

#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>

float * apply3DGaussianFilter(float *** ker, float * map, int dimX, int dimY, int dimZ);
float *** produce3dGaussianKernel(int kernelRadius, double sigma);

#endif //GAUSSIANKERNEL_H