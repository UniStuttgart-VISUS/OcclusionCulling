#ifndef OSMesaPipeline_hpp
#define OSMesaPipeline_hpp

#include <vector>
#include <CPUTMath.h>
#include "CPUTMeshDX11.h"
#include "CPUTCamera.h"

struct GLFWwindow;

class OSMesaPipeline
{
public:
	OSMesaPipeline();
	~OSMesaPipeline();

	void start();
	void GetOccluder(Vertex *vertices, UINT *indices, int numIndices);

	std::vector<Vertex> OccluderSetMP;
	CPUTCamera *cameraMP;

private:
	GLFWwindow * window;

	int mNumIndices = 0;
};
#endif // !OSMesaPipeline_hpp