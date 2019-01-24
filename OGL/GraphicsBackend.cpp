//#include "GraphicsBackend.hpp"
//
//#include <iostream>
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include "linmath.h"
//#include "lodepng/lodepng.h"
//
//static const struct
//{
//	float x, y;
//	float r, g, b;
//}vertices[3] =
//{
//	{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
//	{ 0.6f, -0.4f, 0.f, 1.f, 0.f },
//	{ 0.f,  0.6f, 0.f, 0.f, 1.f }
//};
//
//static const char* vertex_shader_text =
//"#version 110\n"
//"uniform mat4 MVP;\n"
//"attribute vec3 vCol;\n"
////"attribute vec2 vPos;\n"
//"attribute vec3 vPos;\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
////"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
////"    vec3 posNorm = normalize(vec3(vPos, 0.0));\n"
////"    color = vCol;\n"
//"    gl_Position = MVP * vec4(vPos, 1.0);\n"
//"    vec3 posNorm = normalize(vPos);\n"
//"    color = vec3((posNorm.z + 1.f) / 2.f);\n"
//"}\n";
//
//static const char* fragment_shader_text =
//"#version 110\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_FragColor = vec4(color, 1.0);\n"
//"}\n";
//
//
//GraphicsBackend::GraphicsBackend()
//{
//	/* Set context creation api hint to native context */
//	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
//
//	/* Set GPU accelerated output window visible */
//	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
//
//	/* Create a windowed mode window and its OpenGL context */
//	window = glfwCreateWindow(640, 480, "Hello GPU", NULL, NULL);
//	if (window == NULL)
//	{
//		std::cout << "Window creation failed." << std::endl;
//	}
//}
//
//GraphicsBackend::~GraphicsBackend()
//{
//	glfwDestroyWindow(window); //?
//}
//
//void GraphicsBackend::start()
//{
//	/* Make the window's context current */
//	glfwMakeContextCurrent(window);
//
//	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//
//	const unsigned char * vendor;
//	vendor = glGetString(GL_VENDOR);
//	std::cout << "GPU vendor string: " << glGetString(GL_VENDOR) << std::endl;
//
//	// alter data for testing purposes
//	/*OccluderSetGB[0].pos.x = -10.f;
//	OccluderSetGB[0].pos.y = -10.f;
//	OccluderSetGB[0].pos.z = -1.f;
//	OccluderSetGB[1].pos.x = 0.f;
//	OccluderSetGB[1].pos.y = 10.f;
//	OccluderSetGB[1].pos.z = -1.f;
//	OccluderSetGB[2].pos.x = 10.f;
//	OccluderSetGB[2].pos.y = -10.f;
//	OccluderSetGB[2].pos.z = -1.f;*/
//
//	/* Create triangle geometry and simple shader for debugging */
//	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
//	GLint mvp_location, vpos_location, vcol_location;
//	float ratio;
//	int width, height;
//	mat4x4 mvp;
//	char* buffer;
//
//	glGenBuffers(1, &vertex_buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * OccluderSetGB.size(), OccluderSetGB.data(), GL_STATIC_DRAW);
//
//	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
//	glCompileShader(vertex_shader);
//
//	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
//	glCompileShader(fragment_shader);
//
//	program = glCreateProgram();
//	glAttachShader(program, vertex_shader);
//	glAttachShader(program, fragment_shader);
//	glLinkProgram(program);
//
//	mvp_location = glGetUniformLocation(program, "MVP");
//	vpos_location = glGetAttribLocation(program, "vPos");
//	vcol_location = glGetAttribLocation(program, "vCol");
//
//	glEnableVertexAttribArray(vpos_location);
//	//glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);
//	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
//
//	glEnableVertexAttribArray(vcol_location);
//	//glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)(sizeof(float) * 2));
//	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
//
//	glfwGetFramebufferSize(window, &width, &height);
//	ratio = width / (float)height;
//
//	/* Loop until the user closes the window */
//	while (!glfwWindowShouldClose(window))
//	{
//		/* Render here */
//		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//		glViewport(0, 0, width, height);
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		//mat4x4_ortho(mvp, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//
//		mat4x4 proj, view, model;
//		mat4x4_perspective(proj, cameraGB->GetFov(), cameraGB->GetAspectRatio(), cameraGB->GetNearPlaneDistance(), cameraGB->GetFarPlaneDistance());
//		mat4x4_look_at(view, vec3{ cameraGB->GetPosition().x, cameraGB->GetPosition().y ,cameraGB->GetPosition().z }
//						   , vec3{ cameraGB->GetLook().x, cameraGB->GetLook().y ,cameraGB->GetLook().z }
//						   , vec3{ cameraGB->GetUp().x, cameraGB->GetUp().y, cameraGB->GetUp().z });
//		
//		mat4x4_identity(model);
//		mat4x4_mul(mvp, view, model);
//		mat4x4_mul(mvp, proj, mvp);
//
//		glUseProgram(program);
//		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
//		//glDrawArrays(GL_TRIANGLES, 0, 3);
//		glDrawArrays(GL_TRIANGLES, 0, OccluderSetGB.size());
//
//		// Write the offscreen framebuffer to disk for debugging
//		std::vector<unsigned char> image(width*height * 4);
//		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
//		//Encode the image
//		unsigned error = lodepng::encode("testGB.png", image, width, height);
//		//if there's an error, display it
//		if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
//
//		/* Swap front and back buffers */
//		glfwSwapBuffers(window);
//
//		/* Poll for and process events */
//		glfwPollEvents(); //?
//	}
//
//	//glfwTerminate();
//}