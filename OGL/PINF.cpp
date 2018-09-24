#include <future>
#include <iostream>

#include "PINF.hpp"



int PINF::run(const std::vector<Vertex> &vertices)
{

	// Initialize the library
	if (!glfwInit())
		return -1;

	// Create OSMesa pipeline
	OSMesaPipeline mesa_pipe;
	mesa_pipe.OccluderSetMP = vertices;
	mesa_pipe.cameraMP = camera;
	auto mesa_exec = std::async(std::launch::async, &(OSMesaPipeline::start), &mesa_pipe);
	auto mesa_exec_status = mesa_exec.wait_for(std::chrono::microseconds(0));

	GraphicsBackend graphics_backend;
	graphics_backend.OccluderSetGB = vertices;
	graphics_backend.cameraGB = camera;
	auto graphics_exec = std::async(std::launch::async, &(GraphicsBackend::start), &graphics_backend);
	auto graphics_exec_status = graphics_exec.wait_for(std::chrono::microseconds(0));

	while ((mesa_exec_status != std::future_status::ready) || (graphics_exec_status != std::future_status::ready))
	{
		// graphics or mesa pipeline is still running...continue to poll for events
		glfwPollEvents();

		mesa_exec_status = mesa_exec.wait_for(std::chrono::microseconds(2));
		graphics_exec_status = graphics_exec.wait_for(std::chrono::microseconds(2));
	}

	//while ((mesa_exec_status != std::future_status::ready) )
	//{
	//	// graphics or mesa pipeline is still running...continue to poll for events
	//	glfwPollEvents();
	//
	//	mesa_exec_status = mesa_exec.wait_for(std::chrono::microseconds(0));
	//}

	glfwTerminate();

	return 0;
}