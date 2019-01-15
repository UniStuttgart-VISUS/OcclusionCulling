#include "OSMesaPipeline.hpp"

#include <iostream>

#include "osmesa_glad/osmesa_glad.h"
#include <GLFW/glfw3.h>

#include "lodepng/lodepng.h"

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

	osmesa_glEnable(GL_DEPTH_TEST);
	//osmesa_glEnable(GL_BLEND);

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

void OSMesaPipeline::GetOccluder(Vertex *vertices, UINT *indices, int numIndices) {
	for (int i = 0; i < numIndices; ++i) {
		//OccluderSetMP.push_back(vertices[indices[i]]);
	}
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
	// alter data for testing purposes
	/*OccluderSetMP[0].pos.x = -10.f;
	OccluderSetMP[0].pos.y = -10.f;
	OccluderSetMP[0].pos.z = -1.f;
	OccluderSetMP[1].pos.x = -5.f;
	OccluderSetMP[1].pos.y = 10.f;
	OccluderSetMP[1].pos.z = -1.f;
	OccluderSetMP[2].pos.x = 0.f;
	OccluderSetMP[2].pos.y = -10.f;
	OccluderSetMP[2].pos.z = -1.f;

	OccluderSetMP[3].pos.x = 0.f;
	OccluderSetMP[3].pos.y = -10.f;
	OccluderSetMP[3].pos.z = -1.f;
	OccluderSetMP[4].pos.x = 5.f;
	OccluderSetMP[4].pos.y = 10.f;
	OccluderSetMP[4].pos.z = -1.f;
	OccluderSetMP[5].pos.x = 10.f;
	OccluderSetMP[5].pos.y = -10.f;
	OccluderSetMP[5].pos.z = -1.f;

	OccluderSetMP[6].pos.x = 10.f;
	OccluderSetMP[6].pos.y = -10.f;
	OccluderSetMP[6].pos.z = -1.f;
	OccluderSetMP[7].pos.x = 15.f;
	OccluderSetMP[7].pos.y = 10.f;
	OccluderSetMP[7].pos.z = -1.f;
	OccluderSetMP[8].pos.x = 20.f;
	OccluderSetMP[8].pos.y = -10.f;
	OccluderSetMP[8].pos.z = -1.f;*/

	/* Create triangle geometry and simple shader for debugging */
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;
	float ratio;
	int width, height;
	//mat4x4 mvp;
	char* buffer;

	osmesa_glGenBuffers(1, &vertex_buffer);
	osmesa_glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	//osmesa_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	osmesa_glBufferData(GL_ARRAY_BUFFER, sizeof(float4) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

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

	//mvp_location = osmesa_glGetUniformLocation(program, "MVP");
	vpos_location = osmesa_glGetAttribLocation(program, "vPos");

	osmesa_glEnableVertexAttribArray(vpos_location);
	osmesa_glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE, sizeof(float4), (void*)0);


	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;

	osmesa_glViewport(0, 0, width, height);
	osmesa_glClearColor(0.f, 0.f, 0.f, 1.0f);
	osmesa_glClearDepth(1.0);
	osmesa_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//osmesa_glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	osmesa_glDrawBuffer(GL_NONE);

	osmesa_glUseProgram(program);
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