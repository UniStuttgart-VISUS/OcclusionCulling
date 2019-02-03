#include <iostream>
#include <algorithm>

#include "OSMesaPipeline.hpp"

#include "osmesa_glad/osmesa_glad.h"
#include <GLFW/glfw3.h>

#include "lodepng/lodepng.h"

/* Create triangle geometry and simple shader for debugging */
GLuint mVertex_buffer, mVertex_shader, mFragment_shader, mProgram;
GLuint occluder_buffer, occludee_buffer;
GLint mView_location, mProj_location, mModel_location, mVpos_location, mVcol_location;

GLuint *query = new GLuint[MAXNUMQUERIES];

// compute shader declarations
GLuint mCompute_shader;
GLuint mCompute_Program;

static const UINT sBBIndexList[NUMAABBVERTICES] =
{
	// index for top 
	1, 3, 2,
	0, 3, 1,

	// index for bottom
	5, 7, 4,
	6, 7, 5,

	// index for left
	1, 7, 6,
	2, 7, 1,

	// index for right
	3, 5, 4,
	0, 5, 3,

	// index for back
	2, 4, 7,
	3, 4, 2,

	// index for front
	0, 6, 5,
	1, 6, 0,
};

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
//"uniform mat4 model; \n"
"uniform mat4 view; \n"
"uniform mat4 proj; \n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    vec4 temp = (proj * view) * vPos;\n"
"    gl_Position = vec4(temp.x, -temp.y, temp.zw); \n"
//"    vec4 posNorm = normalize(vPos);\n"
//"    color = vec3((posNorm.z + 1.f) / 2.f);\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(vec3(gl_FragCoord.z), 1.0);\n"
"}\n";

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
	window = glfwCreateWindow(mWidth, mHeight, "Hello OSMesa", NULL, NULL);
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

	mVertex_shader = osmesa_glCreateShader(GL_VERTEX_SHADER);
	osmesa_glShaderSource(mVertex_shader, 1, &vertex_shader_text, NULL);
	osmesa_glCompileShader(mVertex_shader);

	mFragment_shader = osmesa_glCreateShader(GL_FRAGMENT_SHADER);
	osmesa_glShaderSource(mFragment_shader, 1, &fragment_shader_text, NULL);
	osmesa_glCompileShader(mFragment_shader);

	mProgram = osmesa_glCreateProgram();
	osmesa_glAttachShader(mProgram, mVertex_shader);
	osmesa_glAttachShader(mProgram, mFragment_shader);
	osmesa_glLinkProgram(mProgram);

	osmesa_glUseProgram(mProgram);

	mVpos_location = osmesa_glGetAttribLocation(mProgram, "vPos");
	//mModel_location = osmesa_glGetAttribLocation(mProgram, "model");
	mView_location = osmesa_glGetUniformLocation(mProgram, "view");
	mProj_location = osmesa_glGetUniformLocation(mProgram, "proj");

	osmesa_glGenQueries(MAXNUMQUERIES, query);

	osmesa_glGenBuffers(1, &occluder_buffer);
}

OSMesaPipeline::~OSMesaPipeline()
{
	glfwDestroyWindow(window); //?
	osmesa_glDeleteBuffers(1, &occluder_buffer);
	osmesa_glDeleteBuffers(1, &occludee_buffer);

	osmesa_glDeleteShader(mVertex_shader);
	osmesa_glDeleteShader(mFragment_shader);
	osmesa_glDeleteProgram(mProgram);

	osmesa_glDeleteQueries(MAXNUMQUERIES, query);

	delete[] query;
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

float* OSMesaPipeline::ConvertMatrix(const float4x4 &matrix) {
	float temp[] = { matrix.r0.x, matrix.r0.y, matrix.r0.z, matrix.r0.w,
					matrix.r1.x, matrix.r1.y, matrix.r1.z, matrix.r1.w,
					matrix.r2.x, matrix.r2.y, matrix.r2.z, matrix.r2.w,
					matrix.r3.x, matrix.r3.y, matrix.r3.z, matrix.r3.w };

	std::copy(std::begin(temp), std::end(temp), std::begin(mat));
	return mat;
}

void OSMesaPipeline::UploadOccluder(const std::vector<float4> &occluder) {
	osmesa_glGenBuffers(1, &occluder_buffer);
	osmesa_glBindBuffer(GL_ARRAY_BUFFER, occluder_buffer);
	osmesa_glBufferData(GL_ARRAY_BUFFER, sizeof(float4) * occluder.size(), occluder.data(), GL_STATIC_DRAW);
}

void OSMesaPipeline::UploadOccludeeAABBs() {
	osmesa_glGenBuffers(1, &occludee_buffer);
	osmesa_glBindBuffer(GL_ARRAY_BUFFER, occludee_buffer);
	osmesa_glBufferData(GL_ARRAY_BUFFER, sizeof(float4) * AABBs.size(), AABBs.data(), GL_STATIC_DRAW);
}

/**
* @param xformedPos  8 vertices of the current AABB
*/
void OSMesaPipeline::GatherAllAABBs(const float4 xformedPos[]) {
	// DepthBuffer remains the same as calculated from RasterizeDepthBuffer() below

	// triangles of AABB
	std::vector<float4> vertices(NUMAABBVERTICES);

	// 12 triangles per AABB
	// should do the same as TransformAABBoxOGL::Gather()
	// store every AABB in array
	for (int i = 0; i < 12; ++i) {
		vertices[i * 3 + 0] = xformedPos[sBBIndexList[i * 3 + 0]];
		vertices[i * 3 + 1] = xformedPos[sBBIndexList[i * 3 + 1]];
		vertices[i * 3 + 2] = xformedPos[sBBIndexList[i * 3 + 2]];
	}

	AABBs.insert(AABBs.end(), vertices.begin(), vertices.end());
}

/**
* Starts Occlusion Queries for each AABB from the current occludee set.
* Tests if bounding boxes pass the depth tests,
* if yes, set true for visibility, false otherwise
*/
void OSMesaPipeline::SartOcclusionQueries(const std::vector<UINT> &ModelIds, const float4x4 &view, const float4x4 &proj) {
	// only bind needed, since occludees are already uploaded
	osmesa_glBindBuffer(GL_ARRAY_BUFFER, occludee_buffer);

	osmesa_glEnableVertexAttribArray(mVpos_location);
	osmesa_glVertexAttribPointer(mVpos_location, 4, GL_FLOAT, GL_FALSE, sizeof(float4), (void*)0);
	
	GLsizei NumQueries = ModelIds.size();
	NumDrawCalls = NumQueries;
	GLuint QueryFinished = 0;
	AABBVisibility.clear();
	AABBVisibility.resize(NumQueries, 1);

	osmesa_glDepthMask(GL_FALSE);

	osmesa_glUniformMatrix4fv(mView_location, 1, GL_FALSE, ConvertMatrix(view));
	osmesa_glUniformMatrix4fv(mProj_location, 1, GL_FALSE, ConvertMatrix(proj));

	// launch queries
	for (int i = 0; i < NumQueries; ++i) {
		// also try GL_ANY_SAMPLES_PASSED_CONSERVATIVE (only if some false positives are acceptable)
		// --> result may be true (some samples passed) but in reality the object is not visible
		osmesa_glBeginQuery(GL_ANY_SAMPLES_PASSED, query[i]);

		// make draw call
		//osmesa_glUniformMatrix4fv(mModel_location, 1, GL_FALSE, ConvertMatrix(model[ModelIds[i]]));
		osmesa_glDrawArrays(GL_TRIANGLES, NUMAABBVERTICES * ModelIds[i], NUMAABBVERTICES);

		osmesa_glEndQuery(GL_ANY_SAMPLES_PASSED);
	}

	// wait until last query result is available
	// all other queries should be also available by then (as stated in the Khronos spec)
	while (!QueryFinished) {
		osmesa_glGetQueryObjectuiv(query[NumQueries - 1], GL_QUERY_RESULT_AVAILABLE, &QueryFinished);
	}

	// get result if last query result is available
	for (int i = 0; i < NumQueries; ++i) {
		osmesa_glGetQueryObjectuiv(query[i], GL_QUERY_RESULT, &AABBVisibility[i]);
	}
}


/**
* Rasterizes the occluder set to the depth buffer
* @param DBTemp  contains the current and the resulting DepthBuffer
* @param vertices  contains the whole occluder geometry
*/
void OSMesaPipeline::RasterizeDepthBuffer(const std::vector<float4> &occluder)
{
	osmesa_glBindBuffer(GL_ARRAY_BUFFER, occluder_buffer);
	osmesa_glBufferData(GL_ARRAY_BUFFER, sizeof(float4) * occluder.size(), occluder.data(), GL_STATIC_DRAW);

	osmesa_glEnableVertexAttribArray(mVpos_location);
	osmesa_glVertexAttribPointer(mVpos_location, 4, GL_FLOAT, GL_FALSE, sizeof(float4), (void*)0);

	// before glClear
	osmesa_glDepthMask(GL_TRUE);

	osmesa_glViewport(0, 0, mWidth, mHeight);
	osmesa_glClearColor(0.f, 0.f, 0.f, 1.f);
	osmesa_glClearDepth(1.0);
	osmesa_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	osmesa_glDrawBuffer(GL_NONE);

	osmesa_glDrawArrays(GL_TRIANGLES, 0, occluder.size());

	// Write the offscreen framebuffer to disk for debugging
	// not used if colormask = GL_FALSE or drawbuffer = GL_NONE
	if (false)
	{
		std::vector<unsigned char> image(mWidth*mHeight * 4);
		osmesa_glReadPixels(0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

		//Encode the image
		unsigned error = lodepng::encode("AAACurrBuffer.png", image, mWidth, mHeight);
		//if there's an error, display it
		if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	}

	/* Swap front and back buffers */
	// glfwSwapBuffers(window);
}

void OSMesaPipeline::GetDepthBuffer(float *DBTemp) {
	osmesa_glReadPixels(0, 0, mWidth, mHeight, GL_DEPTH_COMPONENT, GL_FLOAT, DBTemp);
}