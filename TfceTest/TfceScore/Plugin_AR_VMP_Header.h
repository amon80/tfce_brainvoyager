
#ifndef PLUGIN_AR_VMP_HEADER_H
#define PLUGIN_AR_VMP_HEADER_H


// global data header for anatomically resolved VMPs (arVMPs, interpolated to VMR resolution)
//
struct AR_VMPs_Header
{
	int   NrOfMaps;
	char  FileName[301];

	int   XStart;  // do NOT change right now: if these 6 values are changed, you MUST also change the allocated memory of this and all other VMPs !
	int   XEnd;
	int   YStart;
	int   YEnd;
	int   ZStart;
	int   ZEnd;
	int   Resolution; // MUST be "1" at present

	// Note that X/Y/Z/End values are INCLUSIVE, this means that you must calculate the dimensions of the VMP block as:
	// dim_x = XEndVMP - XStartVMP + 1;
	// dim_y = YEndVMP - YStartVMP + 1;
	// dim_z = ZEndVMP - ZStartVMP + 1;
};

// header for ONE anatomical resolution volume map (arVMP)
//
struct AR_VMP_Header
{
	char  *NameOfMap;

	int   MapType;

	float ThreshMin;
	float ThreshMax;
	int   IncludeValuesGreaterThreshMax; // "1" (default) -> shows map values greater max threshold, "0" excludes values greater max threshold
	int   ClusterSize;
	int   UseClusterSize;
	int   df1;
	int   df2;
	int   ShowPosOrNegOrBoth;   // 1 -> show pos values, 2 -> show neg values, 3 -> show pos and neg valeus
	int   NrOfStatVoxels;

	int   NrOfCrossCorrLags;	// use value only if MapType "3" (cross-correlation or relative contribution map)
	int   CrossCorrMinLag;      // use value only if MapType "3"
	int   CrossCorrMaxLag;		// use value only if MapType "3"
	int   ShowCorrelations;		// use value only if MapType "3", if "1" shows correlation value, if "0" shows lag value in overlayed map

	int   ColorPosMin;
	int   ColorPosMax;
	int   ColorNegMin;
	int   ColorNegMax;
	int   UseMapColor;
	float TransparentColorFactor;

	int   OverlayMap;
};


#endif // PLUGIN_AR_VMP_HEADER_H