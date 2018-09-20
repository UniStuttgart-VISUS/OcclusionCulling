#ifndef OSMesaPipeline_hpp
#define OSMesaPipeline_hpp

#include <vector>
#include <CPUTMath.h>
#include "CPUTMeshDX11.h"

struct GLFWwindow;

class OSMesaPipeline
{
public:
	OSMesaPipeline();
	~OSMesaPipeline();

	void start();
	void Init();
	void CleanUp();
	void GetOccluder(Vertex *vertices, UINT *indices, int numIndices);
	void CreateOccluderSet();

	std::vector<Vertex> mOccluderSet;

private:
	GLFWwindow * window;

	

	int mNumIndices = 0;
};
#endif // !OSMesaPipeline_hpp