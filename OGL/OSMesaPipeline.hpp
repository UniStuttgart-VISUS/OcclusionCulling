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

	void start(std::vector<float4> vertices, float* DBTemp);
	//bool MesaFrustumCull(std::vector<FrustumModel> &CullModels);

	bool singleImage = true;

private:
	GLFWwindow * window;

	void SetMatrixR(mat4x4 &lhs, float4x4 &rhs);
	void SetMatrixP(mat4x4 &lhs, const float4x4 *rhs);

	
};
#endif // !OSMesaPipeline_hpp