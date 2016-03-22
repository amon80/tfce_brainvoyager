
#ifndef PLUGIN_SRF_HEADER_H
#define PLUGIN_SRF_HEADER_H

// helper structs
struct Triangle_Vertices
{ 
	int v1; 
	int v2; 
	int v3; 
}; 

// header for scene of srfs
//
struct SCENE_Header
{
	int NrOfMeshes;
	int CurrentMesh;
};

// header for ONE SRF
//
struct SRF_Header
{
	int   NrOfVertices;
	int   NrOfTriangles;
	
	float *VertexX;
	float *VertexY;
	float *VertexZ;
	float *NormalX;
	float *NormalY;
	float *NormalZ;
	int	  *VertexColor;
	
	int   Visible;
	int   RenderMode;

	float Base1ColorR;  // OpenGL color range [0.0 - 1.0], corresponds to RGB component range [0 - 255]
	float Base1ColorG;
	float Base1ColorB;
	float Base2ColorR;
	float Base2ColorG;
	float Base2ColorB;

	float MeshCenterX;
	float MeshCenterY;
	float MeshCenterZ;
	float MeshRotX;
	float MeshRotY;
	float MeshRotZ;
	
	struct Triangle_Vertices *Triangles;

	int *NrOfVertexNeighbors;
	int **VertexNeighborhood;

	int   UseTriangleStrip;
	int   NrOfStripElements;
	int   *TriangleStrip;

	char  *MeshFileName; // direct ptr to name (260 bytes are reserved)
};

#endif // PLUGIN_SRF_HEADER_H