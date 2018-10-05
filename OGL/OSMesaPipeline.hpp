#ifndef OSMesaPipeline_hpp
#define OSMesaPipeline_hpp

#include <vector>
#include <CPUTMath.h>
#include "CPUTMeshDX11.h"
#include "CPUTCamera.h"

#include "linmath.h"

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
	void SetMatrix(mat4x4 &lhs, const float4x4 *rhs);
};
#endif // !OSMesaPipeline_hpp