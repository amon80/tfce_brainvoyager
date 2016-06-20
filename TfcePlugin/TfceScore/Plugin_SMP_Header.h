
#ifndef PLUGIN_SMP_HEADER_H
#define PLUGIN_SMP_HEADER_H


// header for whole set of SMPs with global params
struct SMPs_Header
{
	int   NrOfSMPs;
	int   NrOfVertices; // same as for SRF, just for checking purpose

	char  SMPFileName[301];
};

// header for ONE SMP
struct SMP_Header
{
	char  *NameOfSMP; // direct access, 101 chars

	int   MapType;
	int   ClusterSize;
	int   UseClusterSize;
	float ThreshMin;
	float ThreshMax;
	int   IncludeValuesGreaterThreshMax; // "1" (default) -> shows map values greater max threshold, "0" excludes values greater max threshold
	int   df1;
	int   df2;

	int   ColorPosMin;
	int   ColorPosMax;
	int   ColorNegMin;
	int   ColorNegMax;
	int   UseSMPColor;
	float TransparentColorFactor;

	int   NrOfMaskVertices;

	int   OverlayMap;
};


#endif // PLUGIN_SMP_HEADER_H
