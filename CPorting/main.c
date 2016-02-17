#include "Tfce.h"
#include "Utilities.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i, j, k;
	int x, y ,z;
	int index;
	float min, max, range;
	float *matrix;
	float *tfce_score_matrix;
	int dim;
	int  xx  =0 , yy = 0, zz = 0;

	FILE *fp;
	fp = fopen("../Test.txt", "r");
	matrix = readMatFromFile(fp, &dim,&xx,&yy,&zz);
	fclose(fp);

	tfce_score_matrix = tfce_score(matrix,xx,yy,zz,0.5,2,0.1);

	/*
	for (int i=0;i<dim;i++){
		if (matrix[i]>0){
			printf("%f\n",matrix[i]);
		}
	}
	*/
	
	findMinMax(tfce_score_matrix, dim, &min, &max, &range);

	printf("\n\n %f \n %f \n %f \n", min, max, range);
	
	fp = fopen("TfceTestC.txt", "w");
	
	for(i = 0; i < dim; i++){
	  fprintf(fp, "%f ", tfce_score_matrix[i]);
	}
	
	fprintf(fp, "\n%d\n%d\n%d\n", xx, yy, zz);
	fclose(fp);

	return 0;
}
