
#ifndef PLUGIN_NR_VMPS_HEADER_H
#define PLUGIN_NR_VMPS_HEADER_H

// global data header for native resolution volume maps (nrVMPs)
//
struct NR_VMPs_Header
{
	int   NrOfMaps;
	char  FileName[301];

	int   NrOfTimePoints;
	int   NrOfMapParams;
	int   ShowParamsFrom;			// per default, all params are shown
	int   ShowParamsTo;				// per default, all params are shown (this can be restricted for display, all params always saved in "ParamsMatrix.txt" file)

	int   UseForICFingerprintFrom; // this is only relevant for ICA-related native resolution VMPs
	int   UseForICFingerprintTo;   // this is only relevant for ICA-related native resolution VMPs

	char   *VTCFileName;			// 261 chars reserved (empty C string ("") if not provided)
	char   *ProtocolFileName;		// 261 chars reserved (emtpy C string ("") if not provided)
	char   *VOIFileName;			// 261 chars reserved (empty C string ("") if not provided)

	int   XStart;  // do NOT change right now: if these 6 values are changed, you MUST also change the allocated memory of this and all other maps !
	int   XEnd;
	int   YStart;
	int   YEnd;
	int   ZStart;
	int   ZEnd;
	int   Resolution; // usually "3" (native resolution of many VTC data sets, but can be also "2" or "1")

	char  **NamesOfComponentParameters; // 51 chars per param

	// Note that X/Y/Z/End values are exclusive (as VTCs/GLMs), compute dims as:
	// dim_x = (XEnd - XStart)/Resolution;
	// dim_y = (YEnd - YStart)/Resolution;
	// dim_z = (ZEnd - ZStart)/Resolution;
};

// header for ONE native resolution volume map (nrVMP)
// FDR table not included (added later in an extra header) 
//
struct NR_VMP_Header
{
	char  *NameOfMap;			// 101 chars

	int   MapType;

	float ThreshMin;
	float ThreshMax;
	int   IncludeValuesGreaterThreshMax; // "1" (default) -> shows map values greater max threshold, "0" excludes values greater max threshold
	int   ClusterSize;
	int   UseClusterSize;
	int   df1;
	int   df2;
	int   NrOfStatVoxels;		// number of voxels with a calculated map value (!= 0.0) - used for multiple comparisons correction
	int   ShowPosOrNegOrBoth;   // 1 -> show pos values, 2 -> show neg values, 3 -> show pos and neg valeus

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

	float *TimeCourse;			// defined only if "(NrOfTimePoints > 0)", otherwise NULL ptr
	float *ComponentParams;		// defined only if "(NrOfComponentParams > 0)", otherwise NULL ptr

	int   OverlayMap;			// get/set whether this component is shown
};

#endif // PLUGIN_NR_VMPS_HEADER_H
