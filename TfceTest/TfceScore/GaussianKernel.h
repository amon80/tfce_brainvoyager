#ifndef GAUSSIANKERNEL_H
#define GAUSSIANKERNEL_H

#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>

typedef std::vector<double> kernel_row;
typedef std::vector<kernel_row> kernel_type;
//std::vector<std::vector<double>>
typedef std::vector<kernel_type> kernel;
//std::vector<std::vector<std::vector<double>>>

void apply3DGaussianFilter(float *** ker, float * map, int dimX, int dimY, int dimZ);
float *** produce3dGaussianKernel(int kernelRadius, double sigma);

#endif //GAUSSIANKERNEL_H