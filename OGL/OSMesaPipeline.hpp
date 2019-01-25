#ifndef OSMesaPipeline_hpp
#define OSMesaPipeline_hpp

#include <vector>
#include <CPUTMath.h>
#include "CPUTMeshDX11.h"
#include "CPUTCamera.h"

#include "linmath.h"

struct GLFWwindow;

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

class OSMesaPipeline
{
public:
	OSMesaPipeline();
	~OSMesaPipeline();

	void start(std::vector<float4> vertices, float* DBTemp);

	void SetFrustumModels(std::vector<FrustumModel> &models) {
		mMesaFrustumModels = models;
	}

private:
	GLFWwindow * window;

	void OccluderFrustumCulling();

	void SetMatrixR(mat4x4 &lhs, float4x4 &rhs);
	void SetMatrixP(mat4x4 &lhs, const float4x4 *rhs);

	std::vector<FrustumModel> mMesaFrustumModels;
};
#endif // !OSMesaPipeline_hpp