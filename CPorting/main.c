#include "Tfce.h"
#include "Utilities.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i, j, k;
	int x, y ,z;
	int index;
	float map[27];

	for (int i =0; i<27; i++){
		map[i] = 1+rand()%10;
		//printf("%f\n",map[i]);
	}

	float *tfce = tfce_score(map,3,3,3,2,1,0.1);
	for (int i =0; i<27; i++){
		//printf("%f\n",tfce[i]);
	}

	float *matrix;
	int dim;
	int  xx  =0 , yy = 0, zz = 0;

	FILE *fp;
	fp = fopen("/Users/Luigi/Documents/MATLAB/ProgettoEsi/Test.txt", "r");
	matrix = readMatFromFile(fp, &dim,&xx,&yy,&zz);
	printf("Dimensione matrice %d\n",dim);

	printf("x:%d y:%d z:%d\n",xx,yy,zz);

	matrix = tfce_score(matrix,xx,yy,zz,2,1,0.1);

	for (int i=0;i<dim;i++){
		if (matrix[i]>0){
			printf("%f\n",matrix[i]);
		}
	}

	return 0;
}
