
#ifndef PLUGIN_VMR_HEADER_H
#define PLUGIN_VMR_HEADER_H

// header(s) for a VMR
//
struct VMR_Header
{
	char  VMRFileName[301];

	int   DimX;
	int   DimY;
	int   DimZ;

	int   OffsetX;
	int   OffsetY;
	int   OffsetZ;
	int   FramingCubeDim;

	float VoxelResolutionX;
	float VoxelResolutionY;
	float VoxelResolutionZ;
	int   VoxelResolutionVerified;
	
	int   TalairachSpace;

	int   LeftRightConvention;    // "1" -> radiological (left-is-right), "2" -> neurological (left-is-left), "0" -> don't know

	int   OriginalV16MinValue;
	int   OriginalV16MeanValue;
	int   OriginalV16MaxValue;
};

struct VMR_ScannerPosition_Header
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

struct VMR_PastTransformations_Header
{
	int   NrOfPastSpatialTransformations;

	char  NameOfSpatialTransformation[10][101];
	int   TypeOfSpatialTransformation[10];
	char  SourceFileOfSpatialTransformation[10][501];
	int   NrOfTransformationValues[10];
	float TransformationValues[10][100];
};


#endif // PLUGIN_VMR_HEADER_H