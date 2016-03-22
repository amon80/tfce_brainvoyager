
#ifndef PLUGIN_VTC_HEADER_H
#define PLUGIN_VTC_HEADER_H


struct VTC_Header
{
	int   HeaderSize;
	int   NrOfVolumes;                  // nr of time points
	int   InMemory;
	int   ReferenceSpace;               // NEW BVQX v2.0 (replaces "InTALSpace"): 1 -> native space, 2 -> ACPC space, 3 -> TAL space ("0" if unknown)
	int   TR;

	int   XStart;
	int   XEnd;
	int   YStart;
	int   YEnd;
	int   ZStart;
	int   ZEnd;
	int   Resolution;                   // "1", "2" or "3" at present

	int   DataType;                     // NEW BVQX v2.0: "1" -> short int (2 bytes), "2" -> float (4 bytes)

	int   RadiologicalConvention;       // "0" -> neurological, "1" -> radiological convention (default, if no header info available)

	char  VTCFileName[261];

	int   NrOfLinkedProtocols;          // NEW in BVQX v2.x";
	char  LinkedProtocolFile[261];		// filled with "current" protocol (if more than one) or empty string ("") if none attached (there will be fns to access other than first protocols)
	char  CreatedFromFMR[261];          // informs about source FMR file

	// Note that X/Y/Z/End values are exclusive, compute dims as:
	// dim_x = (XEndVTC - XStartVTC)/Resolution;
	// dim_y = (YEndVTC - YStartVTC)/Resolution;
	// dim_z = (ZEndVTC - ZStartVTC)/Resolution;
};


#endif // PLUGIN_VTC_HEADER_H