//
// Created by Luigi Giugliano on 14/03/16.
//
#include <math.h>
#include "Utilities.h"

float Fwhm(float *map, int dimX, int dimY, int dimZ) {
	double MM = 0e0, MD = 0e0;

	double S2o = 0e0, S2 = 0e0, Sd2o = 0e0, Sd2 = 0e0, MSa2 = 0e0, Sa2 = 0e0;

	float mSd2;

	int nSd2, nnSd2,i,j,k,z_img,y_img,x_img,onedcord,Newonedcord;

	int dimTot = dimX*dimY*dimZ;


	nnSd2 = 0;

	for (z_img = 0; z_img < dimZ; z_img++) {

		for (y_img = 0; y_img < dimY; y_img++) {

			for (x_img = 0; x_img < dimX; x_img++) {

				onedcord= linearIndexFromCoordinate(x_img,y_img,z_img,dimX,dimY);

				MM += (double) (map[onedcord]);


				mSd2 = 0;

				nSd2 = 0;

				for (k = -1; k <= 1; k++)

					for (j = -1; j <= 1; j++)

						for (i = -1; i <= 1; i++)

							if (((x_img + i) > 0) && ((x_img + i) < dimX) && ((y_img + j) > 0) &&
								((y_img + j) < dimY) && ((z_img + k) > 0) && ((z_img + k) < dimZ)) {

								nSd2++;

								Newonedcord= linearIndexFromCoordinate(x_img + i,y_img + j,z_img + k,dimX,dimY);

								mSd2 += (map[onedcord] - map[Newonedcord]);


								nnSd2++;

								MSa2 += (map[onedcord] - map[Newonedcord]);

							}

				nnSd2--;

				if (nSd2 > 1)

					MD += ((double) (mSd2) / (double) (nSd2 - 1));


			}

		}

	}


	MM /= (double) (dimTot);

	MD /= (double) (dimTot);

	MSa2 /= (double) (nnSd2);


	for (z_img = 0; z_img < dimZ; z_img++) {

		for (y_img = 0; y_img < dimY; y_img++) {

			for (x_img = 0; x_img < dimX; x_img++) {

				onedcord= linearIndexFromCoordinate(x_img,y_img,z_img,dimX,dimY);

				S2o += ((double) (map[onedcord]) * (map[onedcord] - MM)); // 2.2.1

				S2 += ((double) (map[onedcord]) - MM) *
					  ((double) (map[onedcord]) - MM); // 2.3.1

				nSd2 = 0;

				mSd2 = 0;

				for (k = -1; k <= 1; k++)

					for (j = -1; j <= 1; j++)

						for (i = -1; i <= 1; i++)

							if (((x_img + i) > 0) && ((x_img + i) < dimX) && ((y_img + j) > 0) &&
								((y_img + j) < dimY) && ((z_img + k) > 0) && ((z_img + k) < dimZ)) {

								nSd2++;

								Newonedcord= linearIndexFromCoordinate(x_img + i,y_img + j,z_img + k,dimX,dimY);

								mSd2 += (map[onedcord] - map[Newonedcord]);

								Sa2 += (double) (
										(map[onedcord] - map[Newonedcord]) -
										MSa2) * (double) (
										(map[onedcord] - map[Newonedcord]) - MSa2);

							}


				if (nSd2 > 1) {

					//Sd2 += (double)( (mSd2 / (float) (nSd2-1)) * (mSd2 / (float) (nSd2-1)) ) - MD;

					Sd2o += ((double) ((mSd2 / (float) (nSd2 - 1))) - MD) *
							((double) ((mSd2 / (float) (nSd2 - 1))) - MD); // 2.2.1

					Sd2 += (double) ((mSd2 / (float) (nSd2 - 1)) * (mSd2 / (float) (nSd2 - 1))) - MD; // 2.2.1

				}

			}

		}

	}


	S2 /= (double) (dimTot);

	S2o /= (double) (dimTot);


	Sd2 /= (double) (dimTot);

	Sd2o /= (double) (dimTot);


	Sa2 /= (double) (nnSd2);


	float fwhm;

	fwhm = (float) sqrt(-1.0 / (4 * (log(1.0 - (Sd2 / (1e0 * S2o)))))); // 2.2.1

	return  fwhm;

}
