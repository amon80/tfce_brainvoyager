
#ifndef PLUGIN_FMR_HEADER_H
#define PLUGIN_FMR_HEADER_H

// header for FMR-STC
//
struct FMR_Header
{
	char  FileName[301];

	int   NrOfVolumes;					// number of volumes (time points)
	int   DimX;							// number of columns
	int   DimY;							// number of rows
	int   DimZ;							// number of slices

	float VoxelResolutionX;
	float VoxelResolutionY;
	float VoxelResolutionZ;				// SlicThickness + GapThickness
	float SliceThickness;
	float GapThickness;
	int   VoxelResolutionVerified;

	int   TE;							
	int   TR;							
	int   InterSliceTime;				

	int   STCDataType;					// NEW in BVQX v2.x; "1" -> short int (2 bytes), "2" -> float (4 bytes) 

	int   STCStorageFormat;				// "1" or "2" at present; both refer to storage of all repetitions of slice 1, then slice 2 and so on ("1" in different slice files, "2" all data in one file)
	int   STCInMemStorageFormat;		// "0" -> data not in memory, "3" -> access data as "STCData[time][voxel]"
	char  STCPrefix[261];				// Prefix to assess the STC file (e.g. "<Prefix>.stc" contains the data), explicit file access normally not necessary since access fns ensure that data is loaded as needed

	int   NrOfLinkedProtocols;          // NEW in BVQX v2.x";
	char  LinkedProtocolFile[261];		// filled with FIRST protocol (if more than one) or empty string ("") if none attached (there will be fns to access other than first protocols)
	char  LinkedAMRFile[261];
};

struct FMR_ScannerPosition_Header
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

#endif // PLUGIN_FMR_HEADER_H