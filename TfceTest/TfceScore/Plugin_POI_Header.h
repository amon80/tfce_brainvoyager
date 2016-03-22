
#ifndef PLUGIN_POI_HEADER_H
#define PLUGIN_POI_HEADER_H

// header for available POIs of a SRF
//
struct POIs_Header
{
	int  NrOfPOIs;
	int  NrOfPOIMTCs;
	char POIsFileName[261]; // 261 chars reserved (empty C string ("") if not available)
};

// header for ONE POI
//
struct POI_Header
{
	int  NrOfVertices;
	char POIName[256];
	int  DisplayColor;

	int  ReferenceVertex;    // New in BVQX v2.0
	char InfoFileName[261];  // New in BVQX v2.0
};

#endif // PLUGIN_POI_HEADER_H
