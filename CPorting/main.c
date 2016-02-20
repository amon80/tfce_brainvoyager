#include "Tfce.h"
#include "Utilities.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMTHREAD 100

typedef struct{
	double * mat;
	int x;
	int  y;
	int z;
	double E;
	double H;
	double dh;
	double *vet;
	int  i;
} Param, *ParamPtr;

void * run(void * parameters){
	ParamPtr para = parameters;
	double min=0,max=0,range=0,E,H,dh;
	int dim = 0;
	int x,y,z,i;

	x = para->x;
	y = para->y;
	z = para->z;
	i = para->i;

	dim=x*y*z;

	double *matrix = (double *) calloc(sizeof(double),dim);
	double *vetMax;

	memcpy(matrix,para->mat,dim);
	vetMax = para->vet;

	E = para->E;
	H = para->H;
	dh = para->dh;

	printf("THREAD:::dim matrice iniziale: %d\n",dim);
	printf("THREAD:::Indirizzo matrice: %p\n",matrix);
	printf("THREAD:::Indrizzo vetmax: %p indice: %d\n",vetMax,i);
	findMinMax(matrix, dim, &min, &max, &range);
	shuffle(matrix,dim);
	findMinMax(matrix, dim, &min, &max, &range);
	printf("Thread %d: max: %f - min: %f \n", i,max,min);
	double * tfce_score_matrix = tfce_score(matrix,x,y,z,E,H,dh);
	findMinMax(tfce_score_matrix, dim, &min, &max, &range);
	vetMax[i] = max;
	//Para non credo vada liberata adesso, ma nela main, altrimenti facciamo danni
	//free(para);
	free(matrix);
	free(tfce_score_matrix);
	return 0;
}


/*
int createThread(pthread_t thd, double * mat, int x, int  y, int z, double E, double H, double dh, double * vet, int i){

	//printf("Function: %d\n",thd);

}
 */



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
	fp = fopen("/Users/Luigi/Documents/MATLAB/2ndvs1st.txt", "r");
	matrix = readMatFromFile(fp, &dim,&xx,&yy,&zz);
	fclose(fp);

	tfce_score_matrix = tfce_score(matrix,xx,yy,zz,0.5,2,0.1);
	findMinMax(tfce_score_matrix, dim, &min, &max, &range);
	printf("DONE\n");


	double vetmax[NUMTHREAD];
	pthread_t vetThread [NUMTHREAD];

	printf("Indirizzo matirce: %p\n",matrix);
	printf("Indirizzo vet max: %p\n",vetmax);

	ParamPtr para = (ParamPtr) malloc(sizeof(Param));
	for (int i=0;i<NUMTHREAD;i ++){
		//printf("Main: %d\n",vetThread[i]);
		para->mat = matrix;
		para->x = xx;
		para->y = yy;
		para->z = zz;
		para->E = 0.5;
		para->H = 2;
		para->dh = 0.1;
		para->vet = vetmax;
		para->i = i;
		int o=pthread_create(&vetThread[i],NULL,run,para);
		if (o != 0){
			printf("Error creating thread %d \n", i);
		}else {
			printf("Created %p Thread \n", &vetThread[i]);
		}
		//fflush(stdout);
	}

	printf("Created all Thread\n");


	for (int i=0;i<NUMTHREAD; i++) {
		int err =pthread_join(&vetThread[i],NULL);
		printf("error joining thread: %s\n", strerror(err));  //1st optiop

	}
	printf("All thread are finished\n");

	
	findMinMax(vetmax, NUMTHREAD, &min, &max, &range);
	printf("Max: %f Min: %f\n",max,min);

	/*
	//printf("\n\n %lf \n %lf \n %lf \n", min, max, range);
	
	fp = fopen("tfce_score_c.txt", "w");
	
	for (i = 0; i < dim; i++) {
		fprintf(fp, "%lf \n", tfce_score_matrix[i]);
	}	
	fclose(fp);
	*/

	return 0;
}
