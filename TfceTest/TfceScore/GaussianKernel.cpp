#include "GaussianKernel.h"
#include "Utilities.h"

double gaussian(double x, double mu, double sigma) {
	return std::exp(-(((x - mu) / (sigma))*((x - mu) / (sigma))) / 2.0);
}

float *** produce3dGaussianKernel(int kernelRadius, double sigma) {
	//kernel * kernel3d = new kernel(2 * kernelRadius + 1, kernel_type(2 * kernelRadius + 1, kernel_row(2 * kernelRadius + 1)));	
	double sum = 0;
	double x;
	int row, col, height;
	int i, j;
	int dimension = 2 * kernelRadius + 1;
	float *** kernel3d = (float ***) calloc(dimension, sizeof(float **));
	for (i = 0; i < dimension; i++){
		kernel3d[i] = (float **) calloc(dimension, sizeof(float *));
		for (j = 0; j < dimension; j++){
			kernel3d[i][j] = (float *)calloc(dimension, sizeof(float));
		}
	}
	// compute values
	for (row = 0; row < dimension; row++){
		for (col = 0; col < dimension; col++) {
			for (height = 0; height < dimension; height++){
				x = gaussian(row, kernelRadius, sigma)
					* gaussian(col, kernelRadius, sigma)
					* gaussian(height, kernelRadius, sigma);
				kernel3d[row][col][height] = x;
				sum += x;
			}
		}
	}
	// normalize
	for (row = 0; row < dimension; row++){
		for (col = 0; col < dimension; col++) {
			for (height = 0; height < dimension; height++){
				kernel3d[row][col][height] /= sum;
			}
		}
	}
	return kernel3d;
}

void apply3DGaussianFilter(float *** ker, float * map, int dimX, int dimY, int dimZ){
	int n = dimX * dimY * dimZ;
	int i, j, k;
	int linearCoordinate, linearCoordinate2;
	int l, w, q;
	float * new_map = (float *) calloc(sizeof(float), n);
	float kernel_val;
	float sum;

	for (i = 0; i < dimX; i++){
		for (j = 0; i < dimY; j++){
			for (k = 0; k < dimZ; k++){
				linearCoordinate = linearIndexFromCoordinate(i, j, k, dimX, dimY);
				sum = 0;
				for (l = -1; l <= 1; l++){
					for (w = -1; w <= 1; w++){
						for (q = -1; q <= 1; q++){
							if (i + l < 0 || i + l > dimX)
								continue;
							if (j + w < 0 || j + w > dimY)
								continue;
							if (k + q < 0 || k + q > dimZ)
								continue;
							linearCoordinate2 = linearIndexFromCoordinate(i + l, j + w, k + q, dimX, dimY);
							kernel_val = ker[l + 1][w + 1][q + 1];
							sum += kernel_val * map[linearCoordinate2];
						}
					}
				}
				new_map[linearCoordinate] = sum;
			}
		}
	}
	free(map);
	map = new_map;
}