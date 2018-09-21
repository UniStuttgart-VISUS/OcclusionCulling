#ifndef GraphicsBackend_hpp
#define GraphicsBackend_hpp

#include <vector>
#include <CPUTMath.h>
#include "CPUTMeshDX11.h"

struct GLFWwindow;

class GraphicsBackend
{
public:
	GraphicsBackend();
	~GraphicsBackend();

	void start();

	std::vector<Vertex> OccluderSetGB;

private:
	GLFWwindow* window;

};


#endif // !GraphicsBackend_hpp
