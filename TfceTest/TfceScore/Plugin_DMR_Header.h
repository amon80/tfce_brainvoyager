
#ifndef PLUGIN_DMR_HEADER_H
#define PLUGIN_DMR_HEADER_H

// header for DMR
//
struct DMR_Header
{
	char  FileName[301];

	int   NrOfDirectionVolumes;			// nr of directions (including b0, i.e. all data points)
	int   DimX;							// number of columns
	int   DimY;							// number of rows
	int   DimZ;							// number of slices

	float VoxelResolutionX;
	float VoxelResolutionY;
	float VoxelResolutionZ;				// SlicThickness + GapThickness
	float SliceThickness;
	float GapThickness;
	int   VoxelResolutionVerified;

	float **GradientDirections;			// GradientDirections[Vol][3], for each volume, this stores the x, y, z components of the associated gradient direction (0,0,0 for zero-weighted)
	int   GradientDirectionsVerified;	// if "1", the GradientDirections have been set by user or loaded from file, if "0", they might not be set (all dirs are probably 0,0,0)
	char  GradientDirectionsFile[261];	// if available, contains the list of gradient directions (one x, y, z row for each volume)

	int   GradientXDirInterpretation;	// 1: left-to-right, 2: right-to-left, 3: anterior-to-posterior, 4: posterior-to-anterior, 5: inferior-to-superior 6: superior-to-inferior
	int   GradientYDirInterpretation;	// as above
	int   GradientZDirInterpretation;	// as above (Siemens would have entries 1, 3, 5 for X, Y, Z), default will be set to interpreation of X, Y, Z from "ScannerPosition" info (if available)

	float *BValues;						// b value for each gradient direction volume (often the same b value is used for all directions)
	float *SmallDelta;					// not used at present, but probably in future BVQX versions, check for NULL pointer before reading array values)
	float *LargeDelta;					// ditto
	float *GradientAmplitude;			// g value, ditto

	int   TE;							// all scans of one project must have the same TE value (if different TE's used, create multiple DMR/DWI projects)
	int   TR;							// not relevant, just for completeness
	int   InterSliceTime;				// not relevant, just for completeness

	int   DWIDataType;					// NEW in BVQX v2.x; "1" -> short int (2 bytes), "2" -> float (4 bytes) 

	int   DWIStorageFormat;				// "3" or "4": "3" -> a series of direction volumes [directions [voxels]], "4" for each voxel, directions are stored as a continous sequence -> [voxels [directions]]  (formats "1" and "2" are used in FMR-STC projects)
	int   DWIInMemStorageFormat;		// "0" -> data not in memory, "3" -> access data as "DWIData[direction][voxel]", "4" -> access data as "DWIData[voxel][direction]"
	char  DWIPrefix[261];				// Prefix to assess the DWI file (e.g. "<Prefix>.dwi" contains the data), explicit file access normally not necessary since access fns ensure that data is loaded as needed

	int   NrOfLinkedProtocols;          // NEW in BVQX v2.x";
	char  LinkedProtocolFile[261];		// filled with FIRST protocol (if more than one) or empty string ("") if none attached (there will be fns to access other than first protocols)
	char  LinkedAMRFile[261];
};

struct DMR_ScannerPosition_Header
{
	int   MagnetPosInfosVerified;
	int   MagnetCoordinateSystem;
	float MagnetSlice1CenterX;
	float MagnetSlice1CenterY;
	float MagnetSlice1CenterZ;
	float MagnetSliceNCenterX;
	float MagnetSliceNCenterY;
	float MagnetSliceNCenterZ;
	float MagnetRowDirX;
	float MagnetRowDirY;
	float MagnetRowDirZ;
	float MagnetColDirX;
	float MagnetColDirY;
	float MagnetColDirZ;
	int   MagnetNRows;
	int   MagnetNCols;
	float MagnetFoVRows;
	float MagnetFoVCols;
	float MagnetSliceThickness;
	float MagnetGapThickness;
};

#endif // PLUGIN_DMR_HEADER_H