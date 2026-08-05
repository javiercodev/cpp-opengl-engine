#pragma once
#include <ogl3d/math/ovec4.h>
#include <ogl3d/oprerequisites.h>
#include <ogl3d/math/orect.h>
#include <glad/glad.h> 

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
	// Resource Creation:

	// Uploads vertex data (and its attribute layout) to the GPU, returns a VAO wrapping it
	OVertexArrayObjectPtr createVertexArrayObject(const OVertexBufferDesc& data);

	// Allocates a memory buffer for constant/uniform data (e.g. matrices, colors) shared across shaders
	OUniformBufferPtr createUniformBuffer(const OUniformBufferDesc& desc);

	// Compiles + links a vertex/fragment shader pair from disk
	OShaderProgramPtr createShaderProgram(const OShaderProgramDesc& desc);
public:
	// Pipeline State & Commands:

	void clear(const Ovec4& color);
	void setViewport(const ORect& size);          // maps NDC to the given pixel rect
	void setVertexArrayObject(const OVertexArrayObjectPtr& vao); // binds a VAO for drawing

	// Binds a uniform buffer to a specific shader registration slot
	void setUniformBuffer(const OUniformBufferPtr& buffer, ui32 slot);

	void setShaderProgram(const OShaderProgramPtr& program);     // activates a program for drawing

	// Drawing:

	// Executes a draw command. Added OtriangleType to support different primitive topologies 
	// (e.g. Triangles, Triangle Strips, etc.) rather than only list-based triangles.
	void drawTriangles(const OtriangleType& triangleType, ui32 vertexCount, ui32 offset);

};