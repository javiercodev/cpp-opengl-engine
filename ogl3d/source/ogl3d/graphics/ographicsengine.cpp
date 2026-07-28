#include <ogl3d/graphics/ographicsengine.h>
#include <glad/glad.h>
#include <assert.h>
#include <stdexcept>

// Platform-agnostic rendering commands. Context/extension setup lives in
// cwin32graphicsengine.cpp; this file only wraps calls that GLAD already
// exposes as plain OpenGL, with no Win32 dependency.

void OGraphicsEngine::clear(const Ovec4& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}