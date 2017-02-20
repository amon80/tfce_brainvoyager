#ifndef FINDCLUSTERS3D_H
#define FINDCLUSTERS3D_H

#include "BinaryString.h"

int find_clusters_3D(BinaryString& image, int dim_x, int dim_y, int dim_z);
void turn_into_extend_map(BinaryString& extent_map, BinaryString& image, int num_clusters);

#endif //FINDCLUSTERS3D_H
