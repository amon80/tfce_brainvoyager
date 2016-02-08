#include "Utilities.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i, j, k;
	int x, y ,z;
	float matrix [3][3][3];
	float map[27];
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j) {
			for (k = 0; k < 3; ++k) {
				matrix[i][j][k] = rand()%10;
			}
		}
	}

	for (i = 0; i < 27; ++i) {
		coordinatesFromLinearIndex(i, 3, 3, &x, &y, &z);
		map[i] = matrix[x][y][z];
	}	

	return 0;
}
