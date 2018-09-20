#include "GraphicsBackend.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


GraphicsBackend::GraphicsBackend()
{
	
}

GraphicsBackend::~GraphicsBackend()
{
	glfwDestroyWindow(window); //?
}

void GraphicsBackend::start()
{
	/* Set context creation api hint to native context */
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);

	/* Set GPU accelerated output window visible */
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello GPU", NULL, NULL);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	const unsigned char * vendor;
	vendor = glGetString(GL_VENDOR);
	std::cout << "GPU vendor string: " << glGetString(GL_VENDOR) << std::endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents(); //?
	}
}