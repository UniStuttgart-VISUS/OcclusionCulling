#include "OSMesaPipeline.hpp"

#include <iostream>

#include "osmesa_glad/osmesa_glad.h"
#include <GLFW/glfw3.h>

#include "lodepng/lodepng.h"

/* Create triangle geometry and simple shader for debugging */
GLuint mVertex_buffer, mVertex_shader, mFragment_shader, mProgram;
GLint mMVP_location, mVpos_location, mVcol_location;

// compute shader declarations
GLuint mCompute_shader;
GLuint mBlabla;

static const struct
{
	float x, y;
	float r, g, b;
}vertices[6] =
{
	{ -0.f, -1.f, 1.f, 0.f, 0.f },
	{ 0.6f, -1.f, 0.f, 1.f, 0.f },
	{ 0.3f,  0.f, 0.f, 0.f, 1.f },
	{ -0.6f, -1.f, 1.f, 0.f, 0.f },
	{ 0.f, -1.f, 0.f, 1.f, 0.f },
	{ -0.3f,  0.f, 0.f, 0.f, 1.f }
};

static const char* vertex_shader_text =
"#version 110\n"
"attribute vec4 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(vPos.x, -vPos.y, vPos.zw);\n"
//"    vec4 posNorm = normalize(vPos);\n"
//"    color = vec3((posNorm.z + 1.f) / 2.f);\n"
"}\n";

static const char* fragment_shader_text;// =
//"#version 110\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_FragColor = vec4(vec3(gl_FragCoord.z), 1.0);\n"
//"}\n";

static const char* compute_shader_text =
"";

OSMesaPipeline::OSMesaPipeline()
{
	glfwInit();

	/* Set context creation api hint to OSMesa */
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_OSMESA_CONTEXT_API);

	/* Hide window at startup, because we are rendering offscreen anyway */
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1920, 1080, "Hello OSMesa", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window creation failed." << std::endl;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Load OpenGL context with modified glad */
	osmesa_gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//osmesa_glEnable(GL_BLEND);
	osmesa_glEnable(GL_DEPTH_TEST);
	//osmesa_glEnable(GL_CULL_FACE);

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
}

OSMesaPipeline::~OSMesaPipeline()
{
	glfwDestroyWindow(window); //?
}

void OSMesaPipeline::SetMatrixP(mat4x4 &lhs, const float4x4 *rhs) {
	lhs[0][0] = rhs->r0.x;
	lhs[0][1] = rhs->r0.y;
	lhs[0][2] = rhs->r0.z;
	lhs[0][3] = rhs->r0.w;

	lhs[1][0] = rhs->r1.x;
	lhs[1][1] = rhs->r1.y;
	lhs[1][2] = rhs->r1.z;
	lhs[1][3] = rhs->r1.w;

	lhs[2][0] = rhs->r2.x;
	lhs[2][1] = rhs->r2.y;
	lhs[2][2] = rhs->r2.z;
	lhs[2][3] = rhs->r2.w;

	lhs[3][0] = rhs->r3.x;
	lhs[3][1] = rhs->r3.y;
	lhs[3][2] = rhs->r3.z;
	lhs[3][3] = rhs->r3.w;
}

void OSMesaPipeline::SetMatrixR(mat4x4 &lhs, float4x4 &rhs) {
	lhs[0][0] = rhs.r0.x;
	lhs[0][1] = rhs.r0.y;
	lhs[0][2] = rhs.r0.z;
	lhs[0][3] = rhs.r0.w;

	lhs[1][0] = rhs.r1.x;
	lhs[1][1] = rhs.r1.y;
	lhs[1][2] = rhs.r1.z;
	lhs[1][3] = rhs.r1.w;

	lhs[2][0] = rhs.r2.x;
	lhs[2][1] = rhs.r2.y;
	lhs[2][2] = rhs.r2.z;
	lhs[2][3] = rhs.r2.w;

	lhs[3][0] = rhs.r3.x;
	lhs[3][1] = rhs.r3.y;
	lhs[3][2] = rhs.r3.z;
	lhs[3][3] = rhs.r3.w;
}




void OSMesaPipeline::start(std::vector<float4> vertices, float* DBTemp)
{
	float ratio;
	int width, height;

	osmesa_glGenBuffers(1, &mVertex_buffer);
	osmesa_glBindBuffer(GL_ARRAY_BUFFER, mVertex_buffer);
	osmesa_glBufferData(GL_ARRAY_BUFFER, sizeof(float4) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	mVertex_shader = osmesa_glCreateShader(GL_VERTEX_SHADER);
	osmesa_glShaderSource(mVertex_shader, 1, &vertex_shader_text, NULL);
	osmesa_glCompileShader(mVertex_shader);

	mFragment_shader = osmesa_glCreateShader(GL_FRAGMENT_SHADER);
	osmesa_glShaderSource(mFragment_shader, 1, &fragment_shader_text, NULL);
	osmesa_glCompileShader(mFragment_shader);

	mCompute_shader = osmesa_glCreateShader(GL_COMPUTE_SHADER);
	osmesa_glShaderSource(mCompute_shader, 1, &compute_shader_text, NULL);
	osmesa_glCompileShader(mCompute_shader);

	mProgram = osmesa_glCreateProgram();
	osmesa_glAttachShader(mProgram, mVertex_shader);
	osmesa_glAttachShader(mProgram, mFragment_shader);
	osmesa_glAttachShader(mProgram, mCompute_shader);
	osmesa_glLinkProgram(mProgram);

	mVpos_location = osmesa_glGetAttribLocation(mProgram, "vPos");

	osmesa_glEnableVertexAttribArray(mVpos_location);
	osmesa_glVertexAttribPointer(mVpos_location, 4, GL_FLOAT, GL_FALSE, sizeof(float4), (void*)0);

	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;

	osmesa_glViewport(0, 0, width, height);
	osmesa_glClearColor(0.f, 0.f, 0.f, 1.f);
	osmesa_glClearDepth(1.0);
	osmesa_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//osmesa_glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	osmesa_glDrawBuffer(GL_NONE);
	//osmesa_glCullFace(GL_FRONT);

	osmesa_glUseProgram(mProgram);
	osmesa_glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	// read depth buffer
	osmesa_glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, DBTemp);

	// Write the offscreen framebuffer to disk for debugging
	// not used if colormask = GL_FALSE or drawbuffer = GL_NONE
	if (false)
	{
		std::vector<unsigned char> image(width*height * 4);
		osmesa_glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

		//Encode the image
		unsigned error = lodepng::encode("AAACurrBuffer.png", image, width, height);
		//if there's an error, display it
		if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	}

	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	//glfwTerminate();
}