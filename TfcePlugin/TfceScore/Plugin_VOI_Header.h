
#ifndef PLUGIN_VOI_HEADER_H
#define PLUGIN_VOI_HEADER_H

// header for available VOIs with global data
//
struct VOIs_Header
{
	int   NrOfVOIs;
	int   NrOfVOIVTCs;
	char  VOIsFileName[261];          // 261 chars reserved (empty C string ("") if not available)
	
	int   VOIsReferenceSpace;         // NEW in BVQX v2.0 (replaces "VOIsInTalairachCoords"), "1" -> radiological, "2" -> neurological ("0" -> don't know)
	int   SubjectVOINamingConvention; // "1" -> <VOI>_<SUBJECT>, "2" -> <SUBJECT>_<VOI>

	// NEW in BVQX v2.0 - these params are used to check whether VOIs fit to current VMR
	float OriginalVMRResolutionX;
    float OriginalVMRResolutionY;
    float OriginalVMRResolutionZ;
	int   OriginalVMROffsetX;
	int   OriginalVMROffsetY;
	int   OriginalVMROffsetZ;
	int   OriginalVMRFramingCubeDim;
	int   OriginalVMRLeftRightConvention;
};

// header for ONE VOI
//
struct VOI_Header
{
	int  NrOfVoxels;
	char VOIName[256];
	int  DisplayColor;
};

#endif // PLUGIN_VOI_HEADER_H
