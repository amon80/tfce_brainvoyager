#include "Utilities.h"
#include "Tfce.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i, j, k;
	int x, y ,z;
	int index;
	float matrix [20][20][20];
	float map[30];

	for (int i =0; i<30; i++){
		map[i] = rand()%10;
		printf("%f\n",map[i]);
	}

	for (k = 0; k < 3; ++k) {
		for (j = 0; j < 3; ++j) {
			for (i = 0; i < 3; ++i) {
				matrix[i][j][k] = rand()%10;
				//printf("%d\n", linearIndexFromCoordinate(i,j,k,3,3));
			}
		}
	}

	float *tfce = tfce_score(map,20,20,20,1,1,1);
	for (int i =0; i<30; i++){
		printf("%f\n",tfce[i]);
	}
	return 0;
}
