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

struct FrustumModel {
	float4x4 world;
	float4x4 viewprojviewport;
	float3 bbcenterws;
	float3 bbhalfws;
	bool IsVisible = 1;
	bool TooSmall = 0;

	FrustumModel() {}
	FrustumModel(const float4x4 &w, const float4x4 &v, const float3 &c, const float3 &h)
	{
		world = w; viewprojviewport = v; bbcenterws = c; bbhalfws = h;
	}
};

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

	void UploadOccluder(const std::vector<float4> &occluder);
	void UploadOccludeeAABBs();

	void RasterizeDepthBuffer(const std::vector<float4> &vertices);
	void GatherAllAABBs(const float4 xformedPos[], const float4x4 &world);
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
	void OccluderFrustumCulling();

	float* ConvertMatrix(const float4x4 &matrix);
	float mat[16];

	std::vector<float4> mAABBs;
	std::vector<float4x4> mWorldMatrices;

	GLFWwindow * window;
	int mWidth = 1920, mHeight = 1080;

	const UINT BUFFEROFFSET = NUMAABBVERTICES * sizeof(float4);
};
#endif // !OSMesaPipeline_hpp