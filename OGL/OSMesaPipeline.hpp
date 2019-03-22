#ifndef OSMesaPipeline_hpp
#define OSMesaPipeline_hpp

#include <vector>
#include "CPUTCamera.h"
#include "../Constants.h"

#include "linmath.h"

struct GLFWwindow;

const int NUMAABBVERTICES = 36;
const int MAXNUMQUERIES = 27025;


struct ModelAABB {
	std::vector<float4> AABBVerts;

	ModelAABB() {}
	ModelAABB(const std::vector<float4> &verts) : AABBVerts(verts) {}
};

class OSMesaPipeline
{
public:
	OSMesaPipeline(UINT mDBResolution);
	~OSMesaPipeline();

	void UploadOccluder(UINT *VStart);
	void UploadOccludeeAABBs();

	void RasterizeDepthBuffer(const std::vector<float4> &vertices);
	void GatherAllAABBs(const float4 xformedPos[], const float4x4 &world);
	void GatherAllOccluder(const std::vector<float4> &geo, const float4x4 &world);
	void SartOcclusionQueries(const std::vector<UINT> &ModelIds, const float4x4 &view, const float4x4 &proj);
	void SartOcclusionQueries(const UINT ModelIds[], int ModelCount, const float4x4 &view, const float4x4 &proj, UINT idx);

	void GetDepthBuffer(float* DBTemp);

	std::vector<UINT> &GetVisibilityList() {
		return AABBVisibility;
	}

	std::vector<UINT> AABBVisibility;
	UINT *AABBVisible[2];

	bool InitAllOccludeeBB = true;

private:

	float* ConvertMatrix(const float4x4 &matrix);
	float mat[16];

	std::vector<float4> mOccluderGeometry;
	std::vector<float4x4> mWorldMatricesOccluder;

	std::vector<float4> mAABBs;
	std::vector<float4x4> mWorldMatricesAABB;

	GLFWwindow * window;
	int mWidth = 1920, mHeight = 1080;

	const UINT BUFFEROFFSET = NUMAABBVERTICES * sizeof(float4);
};
#endif // !OSMesaPipeline_hpp