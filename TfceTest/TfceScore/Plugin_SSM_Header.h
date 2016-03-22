
#ifndef PLUGIN_SSM_HEADER_H
#define PLUGIN_SSM_HEADER_H

// header for SSM data structure linked to a SRF
//
struct SSM_Header
{
	int   NrOfSourceMeshVertices;
	int   NrOfTargetMeshVertices;
	
	char  *SSMFileName; // direct ptr to name
};

#endif // PLUGIN_SSM_HEADER_H