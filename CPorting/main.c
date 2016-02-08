#include "Utilities.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i, j, k;
	int x, y ,z;
	int index;
	float matrix [3][3][3];
	float map[27];
	for (k = 0; k < 3; ++k) {
		for (j = 0; j < 3; ++j) {
			for (i = 0; i < 3; ++i) {
				matrix[i][j][k] = rand()%10;
				printf("%d\n", linearIndexFromCoordinate(i,j,k,3,3));
			}
		}
	}
	return 0;
}
