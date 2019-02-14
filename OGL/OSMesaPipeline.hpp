#ifndef OSMesaPipeline_hpp
#define OSMesaPipeline_hpp

#include <vector>
#include <CPUTMath.h>
#include "CPUTMeshDX11.h"
#include "CPUTCamera.h"

#include "linmath.h"


struct GLFWwindow;

const int NUMAABBVERTICES = 36;
const int MAXNUMQUERIES = 27025;
const int MAXNUMOCCLUDER = 115;


struct ModelAABB {
	std::vector<float4> AABBVerts;

	ModelAABB() {}
	ModelAABB(const std::vector<float4> &verts) : AABBVerts(verts) {}
};

class OSMesaPipeline
{
public:
	OSMesaPipeline();
	~OSMesaPipeline();

	void UploadOccluder(const UINT NumModels);
	void UploadOccludeeAABBs();

	void RasterizeDepthBuffer(const std::vector<float4> &vertices);
	void RasterizeDepthBuffer(UINT *OccluderId, const float4x4 &view, const float4x4 &proj, UINT NumModels);
	void GatherAllAABBs(const float4 xformedPos[], const float4x4 &world);
	void GatherAllOccluder(const std::vector<float4> geo, const float4x4 &world, const UINT NumTriangles);
	void SartOcclusionQueries(const std::vector<UINT> &ModelIds, const float4x4 &view, const float4x4 &proj);
	void SartOcclusionQueries(const UINT ModelIds[], int ModelCount, const float4x4 &view, const float4x4 &proj, UINT idx);

	void GetDepthBuffer(float* DBTemp);

	std::vector<UINT> &GetVisibilityList() {
		return AABBVisibility;
	}

	std::vector<UINT> AABBVisibility;
	UINT *AABBVisible[2];

	bool InitAllOccludeeBB = true;
	bool InitAllOccluder = true;

private:
	void OccluderFrustumCulling();

	float* ConvertMatrix(const float4x4 &matrix);
	float mat[16];

	void CalcOccluderOffsets(const UINT offsets);
	std::vector<float4> mOccluderGeometry;
	std::vector<float4x4> mOccluderWorldMatrices;
	std::vector<UINT> mNumTriangles;
	std::vector<UINT> mOccluderBufferOffset;

	std::vector<float4> mAABBs;
	std::vector<float4x4> mWorldMatricesAABB;

	GLFWwindow * window;
	int mWidth = 1920, mHeight = 1080;

	const UINT BUFFEROFFSET = NUMAABBVERTICES * sizeof(float4);
};
#endif // !OSMesaPipeline_hpp