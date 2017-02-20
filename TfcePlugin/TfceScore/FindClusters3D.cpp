#include "FindClusters3D.h"
#include <queue>
#include "StatisticalMap3D.h"

//converts 3D to 1D index
int linearIndexFromCoordinate(int x, int y, int z, int max_x, int max_y){
	return(z*max_x*max_y + y*max_x + x);
}

//converts 1D to 3D index
void coordinatesFromLinearIndex(int index, int max_x, int max_y, int& x, int& y, int& z){
	x =  index % (max_x);
	index /= (max_x);
	y = index % (max_y);
	index /= (max_y);
	z = index;
}

int find_clusters_3D(BinaryString& image, int dim_x, int dim_y, int dim_z){
    if(dim_x * dim_y * dim_z != image.size())
        return -1;
    int label = 2;
	int x, y, z;
	int toCheck;
	int i, j, k, h;
	int actual_index;
	std::queue<int> q;
	int num_clusters = 0;
	BinaryString toReturn(image);
    int n = image.size();
	for (i = 0; i < n; ++i) {
		if (toReturn[i] == 1) {
			q.push(i);
			toReturn[i] = label;
			num_clusters++;
			while(!q.empty()){
				actual_index = q.front();
				q.pop();
				x = 0; y = 0; z = 0;
				coordinatesFromLinearIndex(actual_index, dim_x, dim_y, x, y, z);			
				for (j = -1; j <= 1; ++j) {
					if (x+j < 0)
						continue;	
					if (x+j >= dim_x)
						continue;
					for (k = -1; k <= 1; ++k) {
						if (y+k < 0)
							continue;	
						if (y+k >= dim_y)
							continue;
						for (h = -1; h <= 1; ++h) {
							if (z+h < 0) 
								continue;	
							if (z+h >= dim_z)
								continue;
							toCheck = linearIndexFromCoordinate(x+j, y+k, z+h, dim_x, dim_y);	
							if (toReturn[toCheck] == 1) {
								q.push(toCheck);
								toReturn[toCheck] = label;
							}
						}	
					}	
				}
			}
			label++;
		}
	}
	for (i = 0; i < n; ++i) {
		if (toReturn[i] != 0) 
			toReturn[i]--;
	}
	image = toReturn;
	return num_clusters;
}

void turn_into_extend_map(BinaryString& extent_map, BinaryString& clustered_map, int num_clusters){
    int i,j;
    int n = clustered_map.size();
    int numOfElementsMatching;
    for (i = 1; i <= num_clusters; ++i) {
		numOfElementsMatching = 0;	
		for (j = 0; j < n; ++j){
			if(clustered_map[j] == i){
				numOfElementsMatching++;
			}
		}
		for (j = 0; j < n; ++j) {
			if(clustered_map[j] == i){
				extent_map[j] = numOfElementsMatching;
			}	
		}
	}
}
