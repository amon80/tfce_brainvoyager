
#ifndef PLUGIN_MTC_HEADER_H
#define PLUGIN_MTC_HEADER_H

// header for MTC
struct MTC_Header
{
	int   HeaderSize;
	int   NrOfTimePoints;
	int   NrOfVertices;
	int   DataType; //  1 -> float (default), future data types (not yet tested): 2 -> unsigned short, 3 -> int
	int   InMemory;
	int   TR;

	char  MTCFileName[261];
	char  LinkedProtocolName[261];
	char  CreatedFromVTC[261];
};


#endif // PLUGIN_MTC_HEADER_H