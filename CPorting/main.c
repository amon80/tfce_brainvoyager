#include "Tfce.h"
#include "Utilities.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i, j, k;
	int x, y ,z;
	int index;
	double min, max, range;
	double *matrix;
	double *tfce_score_matrix;
	int dim;
	int logging = 1;
	int  xx  =0 , yy = 0, zz = 0;

	FILE *fp;
	fp = fopen("../../2ndvs1st.txt", "r");
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

	//printf("\n\n %lf \n %lf \n %lf \n", min, max, range);
	
	fp = fopen("tfce_score_c.txt", "w");
	
	for (i = 0; i < dim; i++) {
		fprintf(fp, "%lf \n", tfce_score_matrix[i]);
	}	
	fclose(fp);

	return 0;
}
