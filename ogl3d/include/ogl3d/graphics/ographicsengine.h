#pragma once
#include <ogl3d/math/ovec4.h>
#include <ogl3d/oprerequisites.h>
#include <ogl3d/math/orect.h>

// Owns the OpenGL context creation and exposes basic render commands.
// Construction sets up a throwaway "dummy" window/context just to load
// the WGL extensions (see cwin32graphicsengine.cpp) — the real rendering
// context lives on OWindow, not here.
class OGraphicsEngine
{
public:
	OGraphicsEngine();
	~OGraphicsEngine();
public:
	// Uploads vertex data to the GPU and returns a VAO wrapping it
	OVertexArrayObjectPtr createVertexArrayObject(const OVertexBufferData& data);
	// Compiles + links a vertex/fragment shader pair from disk
	OShaderProgramPtr createShaderProgram(const OShaderProgramDesc& desc);
public:
	void clear(const Ovec4& color);
	void setViewport(const ORect& size);          // maps NDC to the given pixel rect
	void setVertexArrayObject(const OVertexArrayObjectPtr& vao); // binds a VAO for drawing
	void setShaderProgram(const OShaderProgramPtr& program);     // activates a program for drawing
	void drawTriangles(ui32 vertexCount, ui32 offset);
};