#include "Tfce.h"
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
		printf("%f\n",map[i]);
	}

	float *tfce = tfce_score(map,3,3,3,2,1,0.1);
	for (int i =0; i<27; i++){
		printf("%f\n",tfce[i]);
	}
	return 0;
}
