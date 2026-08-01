#include <ogl3d/game/ogame.h>
#include <ogl3d/window/owindow.h>
#include <ogl3d/graphics/ographicsengine.h>
#include <glad/glad.h>
#include <iostream>

OGame::OGame()
{
	// Graphics engine must exist before the window: it loads the WGL
	// extensions (via a dummy context) that OWindow's constructor relies on.
	m_graphicsEngine = std::make_unique<OGraphicsEngine>();
	m_display = std::make_unique<OWindow>();

	m_display->makeCurrentContext();

	m_graphicsEngine->setViewport(m_display->getInnerSize());
}

OGame::~OGame()
{
}

void OGame::onCreate()
{
	// A single triangle, positions only (x, y, z) — no color/UV attributes yet
	const f32 triangleVertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	m_triangleVAO = m_graphicsEngine->createVertexArrayObject({
		(void*)triangleVertices,
		sizeof(f32) * 3, // bytes per vertex (3 floats)
		3                // vertex count
		});

	// Shaders now live in their own files (previously inlined as raw strings)
	m_shader = m_graphicsEngine->createShaderProgram({
		L"assets/shaders/basicshader.vert",
		L"assets/shaders/basicshader.frag"
		});
}

void OGame::onUpdate()
{
	m_graphicsEngine->setShaderProgram(m_shader);
	m_graphicsEngine->setVertexArrayObject(m_triangleVAO);
	m_graphicsEngine->drawTriangles(3, 0);
}

void OGame::onQuit()
{
}

void OGame::quit()
{
	m_isRunning = false;
}