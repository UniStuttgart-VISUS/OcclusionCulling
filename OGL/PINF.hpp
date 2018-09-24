#ifndef PINF_HPP
#define PINF_HPP

#include <GLFW/glfw3.h>
#include "GraphicsBackend.hpp"
#include "OSMesaPipeline.hpp"

class PINF {
public:
	int run(const std::vector<Vertex> &vertices);
	CPUTCamera *camera;
};

#endif // PINF_HPP