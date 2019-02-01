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
	void GatherAllAABBs(const float4 xformedPos[]);
	void SartOcclusionQueries(const std::vector<UINT> &ModelIds, const float4x4 &view, const float4x4 &proj);

	void GetDepthBuffer(float* DBTemp);

	std::vector<UINT> &GetVisibilityList() {
		return AABBVisibility;
	}

	/*std::vector<int[2]> GetVisibilityIndices() {
		return AABBVisibilityIndex;
	}*/

	std::vector<UINT> AABBVisibility;
	UINT NumDrawCalls = 0;

	bool InitAllOccludeeBB = true;

private:
	GLFWwindow * window;
	int mWidth = 1920, mHeight = 1080;
	
	void OccluderFrustumCulling();

	void SetMatrixR(mat4x4 &lhs, float4x4 &rhs);
	void SetMatrixP(mat4x4 &lhs, const float4x4 *rhs);
	float* ConvertMatrix(const float4x4 &matrix);
	float mat[16];

	std::vector<float4> AABBs;

	const UINT BUFFEROFFSET = NUMAABBVERTICES * sizeof(float4);
};
#endif // !OSMesaPipeline_hpp