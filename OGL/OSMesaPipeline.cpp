#include "OSMesaPipeline.hpp"

#include <iostream>

#include "osmesa_glad/osmesa_glad.h"
#include <GLFW/glfw3.h>

#include "linmath.h"
#include "lodepng/lodepng.h"

static const struct
{
	float x, y;
	float r, g, b;
} vertices[3] =
{
	{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
{ 0.6f, -0.4f, 0.f, 1.f, 0.f },
{ 0.f,  0.6f, 0.f, 0.f, 1.f }
};

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

OSMesaPipeline::OSMesaPipeline()
{
	/* Set context creation api hint to OSMesa */
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_OSMESA_CONTEXT_API);

	/* Hide window at startup, because we are rendering offscreen anyway */
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello OSMesa", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window creation failed." << std::endl;
	}
}

OSMesaPipeline::~OSMesaPipeline()
{
	glfwDestroyWindow(window); //?
}

void OSMesaPipeline::Init() {
	/*mpOccluderIndices = 0;
	mpOccluderIndices = new UINT[];

	mpOccluderVertices = 0;
	mpOccluderVertices = new Vertex[];*/

	/*mpOccluderSet = 0;
	mpOccluderSet = new Vertex[48700 * 3];*/
}

void OSMesaPipeline::CleanUp() {
	/*delete[] mpOccluderIndices; mpOccluderIndices = NULL;
	delete[] mpOccluderVertices; mpOccluderVertices = NULL;
	delete[] mpOccluderSet; mpOccluderSet = NULL;*/
}

void OSMesaPipeline::GetOccluder(Vertex *vertices, UINT *indices, int numIndices) {
	for (int i = 0; i < numIndices; ++i) {
		Vertex check = vertices[indices[i]];
		mOccluderSet.push_back(vertices[indices[i]]);
	}
}

void OSMesaPipeline::CreateOccluderSet() {
	for (int i = 0; i < mNumIndices; ++i) {
		//&mpOccluderSet[i] = mpOccluderVertices[mpOccluderIndices[i]];

	}
}

void OSMesaPipeline::start()
{
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Load OpenGL context with modified glad */
	osmesa_gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	/* Get vendor (and renderer) string to verify the OSMesa context creation */
	const unsigned char * vendor;
	vendor = osmesa_glGetString(GL_VENDOR);
	if (vendor != NULL)
		std::cout << "OSMesa vendor string: " << vendor << std::endl;
	else
		std::cout << "OSMesa vendor string is NULL." << std::endl;

	const unsigned char * renderer;
	renderer = osmesa_glGetString(GL_RENDERER);
	if (renderer != NULL)
		std::cout << "OSMesa renderer string: " << renderer << std::endl;
	else
		std::cout << "OSMesa renderer string is NULL." << std::endl;

	// Get context version information
	int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);

	std::cout << "OSMesa OpenGL context " << major << "." << minor << std::endl;

	/* Create triangle geometry and simple shader for debugging */
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;
	float ratio;
	int width, height;
	mat4x4 mvp;
	char* buffer;

	osmesa_glGenBuffers(1, &vertex_buffer);
	osmesa_glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	osmesa_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vertex_shader = osmesa_glCreateShader(GL_VERTEX_SHADER);
	osmesa_glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	osmesa_glCompileShader(vertex_shader);

	fragment_shader = osmesa_glCreateShader(GL_FRAGMENT_SHADER);
	osmesa_glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	osmesa_glCompileShader(fragment_shader);

	program = osmesa_glCreateProgram();
	osmesa_glAttachShader(program, vertex_shader);
	osmesa_glAttachShader(program, fragment_shader);
	osmesa_glLinkProgram(program);

	mvp_location = osmesa_glGetUniformLocation(program, "MVP");
	vpos_location = osmesa_glGetAttribLocation(program, "vPos");
	vcol_location = osmesa_glGetAttribLocation(program, "vCol");

	osmesa_glEnableVertexAttribArray(vpos_location);
	osmesa_glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
		sizeof(vertices[0]), (void*)0);
	osmesa_glEnableVertexAttribArray(vcol_location);
	osmesa_glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
		sizeof(vertices[0]), (void*)(sizeof(float) * 2));

	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;

	while (!glfwWindowShouldClose(window))
	{
		osmesa_glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		osmesa_glViewport(0, 0, width, height);
		osmesa_glClear(GL_COLOR_BUFFER_BIT);

		mat4x4_ortho(mvp, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		
		osmesa_glUseProgram(program);
		osmesa_glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
		osmesa_glDrawArrays(GL_TRIANGLES, 0, 3);
		
		//TODO write to ring buffer

		// Write the offscreen framebuffer to disk for debugging
		std::vector<unsigned char> image(width*height*4);
		osmesa_glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
		//Encode the image
		unsigned error = lodepng::encode("test.png", image, width, height);
		//if there's an error, display it
		if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	}
}