#include <ogl3d/graphics/ographicsengine.h>
#include <ogl3d/graphics/overtexarrayobject.h>
#include <ogl3d/graphics/oshaderprogram.h>
#include <glad/glad.h>

// Platform-agnostic rendering commands. Context/extension setup lives in
// cwin32graphicsengine.cpp; this file only wraps calls that GLAD already
// exposes as plain OpenGL, with no Win32 dependency.

OVertexArrayObjectPtr OGraphicsEngine::createVertexArrayObject(const OVertexBufferDesc& data)
{
	return std::make_shared<OVertexArrayObject>(data);
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

void OGraphicsEngine::setShaderProgram(const OShaderProgramPtr& program)
{
	glUseProgram(program->getId());
}

void OGraphicsEngine::drawTriangles(ui32 vertexCount, ui32 offset)
{
	glDrawArrays(GL_TRIANGLES, offset, vertexCount);
}