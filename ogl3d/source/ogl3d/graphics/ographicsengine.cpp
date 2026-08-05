#include <ogl3d/graphics/ographicsengine.h>
#include <ogl3d/graphics/overtexarrayobject.h>
#include <ogl3d/graphics/ouniformbuffer.h>
#include <ogl3d/graphics/oshaderprogram.h>
#include <glad/glad.h>


// Platform-agnostic rendering commands. Context/extension setup lives in
// cwin32graphicsengine.cpp; this file only wraps calls that GLAD already
// exposes as plain OpenGL, with no Win32 dependency.

OVertexArrayObjectPtr OGraphicsEngine::createVertexArrayObject(const OVertexBufferDesc& data)
{
	return std::make_shared<OVertexArrayObject>(data);
}

// New factory method to instantiate Uniform Buffer Objects
OUniformBufferPtr OGraphicsEngine::createUniformBuffer(const OUniformBufferDesc& desc)
{
	return std::make_shared<OUniformBuffer>(desc);
}

OShaderProgramPtr OGraphicsEngine::createShaderProgram(const OShaderProgramDesc& desc)
{
	return std::make_shared<OShaderProgram>(desc);
}

void OGraphicsEngine::clear(const Ovec4& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OGraphicsEngine::setViewport(const ORect& size)
{
	glViewport(size.left, size.top, size.width, size.height);
}

void OGraphicsEngine::setVertexArrayObject(const OVertexArrayObjectPtr& vao)
{
	glBindVertexArray(vao->getId());
}

// New binding logic: Connects the buffer ID to a specific indexed binding point (slot).
// This must align with the slot defined via OShaderProgram::setUniformBufferSlot.
void OGraphicsEngine::setUniformBuffer(const OUniformBufferPtr& buffer, ui32 slot)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, buffer->getId());
}

void OGraphicsEngine::setShaderProgram(const OShaderProgramPtr& program)
{
	glUseProgram(program->getId());
}

// Updated draw command: Now translates engine-agnostic OtriangleType into 
// specific OpenGL primitive enums.
void OGraphicsEngine::drawTriangles(const OtriangleType& triangleType, ui32 vertexCount, ui32 offset)
{
	auto glTriType = GL_TRIANGLES;

	switch (triangleType)
	{
	case TriangleList: { glTriType = GL_TRIANGLES; break; }
	case TriangleStrip: { glTriType = GL_TRIANGLE_STRIP; break; }
	}

	glDrawArrays(glTriType, offset, vertexCount);
}