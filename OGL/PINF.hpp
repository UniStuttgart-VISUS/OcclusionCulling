#ifndef PINF_HPP
#define PINF_HPP

#include <GLFW/glfw3.h>
#include "GraphicsBackend.hpp"
#include "OSMesaPipeline.hpp"

class PINF {
public:
	int run(const std::vector<Vertex> &vertices);// , OSMesaPipeline &mesa_pipe, GraphicsBackend &graphics_backend);
	CPUTCamera *camera;

	std::vector<int> numIndicesPerObjectPINF;
	std::vector<float4x4*> worldMatrixPerObjectPINF;
	std::vector<int> meshCountPerModelPINF;
};

#endif // PINF_HPP