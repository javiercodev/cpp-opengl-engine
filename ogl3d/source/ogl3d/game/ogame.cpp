#include <ogl3d/game/ogame.h>
#include <ogl3d/window/owindow.h>
#include <ogl3d/graphics/ographicsengine.h>
#include <ogl3d/graphics/overtexarrayobject.h>
#include <ogl3d/graphics/oshaderprogram.h>
#include <ogl3d/graphics/ouniformbuffer.h>
#include <glad/glad.h>
#include <iostream>

// Application-side representation of the constant buffer in the shader.
// Structure padding should be considered for more complex data types.
struct UniformData
{
	f32 scale;
};

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
	// Quad definition using 4 vertices for TriangleStrip usage.
	// Vertex layout remains interleaved: Position (3f) + Color (3f)
	const f32 polygonVertices[] = {
		-0.5f, -0.5f, 0.0f,   1, 0, 0, // bottom-left,  red
		-0.5f,  0.5f, 0.0f,   0, 1, 0, // top-left,     green
		 0.5f, -0.5f, 0.0f,   0, 0, 1, // bottom-right, blue
		 0.5f,  0.5f, 0.0f,   1, 1, 0  // top-right,    yellow
	};

	OVertexAttribute attribsList[] = {
		{3}, // position attribute slot
		{3}  // color attribute slot
	};

	m_polygonVAO = m_graphicsEngine->createVertexArrayObject({
		(void*)polygonVertices,
		sizeof(f32) * 6, // Stride: bytes per vertex
		4,                // Total vertices
		attribsList,
		2
		});

	// Initialize the Uniform Buffer to match the size of our CPU-side struct
	m_uniform = m_graphicsEngine->createUniformBuffer({
		sizeof(UniformData)
		});

	m_shader = m_graphicsEngine->createShaderProgram({
		L"assets/shaders/basicshader.vert",
		L"assets/shaders/basicshader.frag"
		});

	// Link the shader's internal block "UniformData" to the hardware slot 0
	m_shader->setUniformBufferSlot("UniformData", 0);
}

void OGame::onUpdate()
{
	// Timing Logic: Calculate Delta Time to ensure frame-rate independent animations.
	auto currentTime = std::chrono::system_clock::now();
	auto elapsedSeconds = std::chrono::duration<double>();

	// Skip calculation on the very first frame to avoid massive delta jumps
	if (m_previousTime.time_since_epoch().count())
		elapsedSeconds = currentTime - m_previousTime;
	m_previousTime = currentTime;

	auto deltaTime = (f32)elapsedSeconds.count();

	// Animate scale factor over time using a sine wave for a pulsing effect
	m_scale += 3.14f * deltaTime;
	auto currentScale = abs(sin(m_scale));

	// Update UBO content with the new calculation before drawing
	UniformData data = { currentScale };
	m_uniform->setData(&data);

	m_graphicsEngine->setShaderProgram(m_shader);
	m_graphicsEngine->setVertexArrayObject(m_polygonVAO);

	// Pipeline Synchronization:
	// The Uniform Buffer must be bound to slot 0 to match the setUniformBufferSlot call above.
	// Switched draw call to TriangleStrip to render the 4-vertex quad correctly.
	m_graphicsEngine->setUniformBuffer(m_uniform, 0);
	m_graphicsEngine->drawTriangles(TriangleStrip, m_polygonVAO->getVertexBufferSize(), 0);
}

void OGame::onQuit()
{
}

void OGame::quit()
{
	m_isRunning = false;
}