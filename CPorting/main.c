#include "Tfce.h"
#include "Utilities.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct param{
	double * mat;
	int x;
	int  y;
	int z;
	double E;
	double H;
	double dh;
	double *vet;
	int  i;
};

void * run(void * param){

	struct param* para= (struct param*) param;
	double min,max,range;
	double dim = para->x*para->y*para->z;
	findMinMax(para->mat, dim, &min, &max, &range);
	double * shuffled = shuffle(para->mat,dim);
	findMinMax(shuffled, dim, &min, &max, &range);
	double * tfce_score_matrix = tfce_score(shuffled,para->x,para->y,para->z,para->E,para->H,para->dh);
	findMinMax(tfce_score_matrix, dim, &min, &max, &range);
	//printf("Thread %d: max: %f - min: %f \n", para->i,max,min);
	//para->vet[para->i] = max;
	return NULL;
}


int createThread(pthread_t thd, double * mat, int x, int  y, int z, double E, double H, double dh, double * vet, int i){

	//printf("Function: %d\n",thd);
	struct param para;
		para.mat = mat;
		para.x = x;
		para.y = y;
		para.z = z;
		para.E = E;
		para.H = H;
		para.dh = dh;
		para.vet = vet;
		para.i = i;
	void * topass = &para;
	int ret = pthread_create(&thd,NULL,run,topass);
	return ret;
}



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


	int numThread = 100;
	double * vetmax = (double *) calloc(sizeof(double),numThread);
	vetmax = fill0(numThread);
	pthread_t vetThread[numThread];

	for (int i=0;i<numThread;i ++){
		//printf("Main: %d\n",vetThread[i]);
		int o = createThread(vetThread[i],matrix,xx,yy,zz,0.5,2,0.1,vetmax,i);
		printf("\nThread %d val %d\n",i ,o);
		if (o != 0){
			printf("error creating thread\n");
		}else {
			printf("Create %d Thread \n", vetThread[i]);
		}
		//fflush(stdout);
	}

	printf("Create all Thread\n");


	for (int i=0;i<numThread; i++) {
		int err =pthread_join(vetThread[i],NULL);
		printf("error joining thread: %s\n", strerror(err));  //1st optiop

	}
	printf("All thread are finished\n");

	
	findMinMax(vetmax, numThread, &min, &max, &range);
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
