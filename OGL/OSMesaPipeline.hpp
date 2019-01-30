#ifndef OSMesaPipeline_hpp
#define OSMesaPipeline_hpp

#include <vector>
#include <CPUTMath.h>
#include "CPUTMeshDX11.h"
#include "CPUTCamera.h"

#include "linmath.h"

struct GLFWwindow;

const int NUMAABBVERTICES = 36;

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

	void RasterizeDepthBuffer(const std::vector<float4> &vertices, float* DBTemp);
	void GatherAllAABBs(const float4 xformedPos[], int id);
	bool SartOcclusionQueries();

	void SetFrustumModels(std::vector<FrustumModel> &models) {
		mMesaFrustumModels = models;
	}

	/*std::vector<int[2]> GetVisibilityIndices() {
		return AABBVisibilityIndex;
	}*/

private:
	GLFWwindow * window;
	

	void OccluderFrustumCulling();

	void SetMatrixR(mat4x4 &lhs, float4x4 &rhs);
	void SetMatrixP(mat4x4 &lhs, const float4x4 *rhs);

	std::vector<float4> AABBs;
	std::vector<int> AABBIndexList;
	std::vector<int[2]> AABBVisibilityIndex;
	std::vector<FrustumModel> mMesaFrustumModels;

	const GLint BUFFEROFFSET = NUMAABBVERTICES * sizeof(float4);
};
#endif // !OSMesaPipeline_hpp