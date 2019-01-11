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

	void start(std::vector<float4> vertices);
	void GetOccluder(Vertex *vertices, UINT *indices, int numIndices);

	std::vector<float4> mOccluderSet;
	bool singleImage = true;

	/*std::vector<Vertex> OccluderSetMP;
	std::vector<int> numIndicesPerObjectMP;
	std::vector<float4x4*> worldMatrixPerObjectMP;
	std::vector<int> meshCountPerModelMP;
	CPUTCamera *cameraMP;*/

private:
	GLFWwindow * window;

	int mNumIndices = 0;
	void SetMatrixR(mat4x4 &lhs, float4x4 &rhs);
	void SetMatrixP(mat4x4 &lhs, const float4x4 *rhs);

	
};
#endif // !OSMesaPipeline_hpp