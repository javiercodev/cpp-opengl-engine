#include <ogl3d/game/ogame.h>
#include <ogl3d/window/owindow.h>
#include <ogl3d/graphics/ographicsengine.h>
#include <ogl3d/graphics/overtexarrayobject.h>
#include <ogl3d/graphics/oshaderprogram.h>
#include <ogl3d/graphics/ouniformbuffer.h>
#include <glad/glad.h>
#include <iostream>
#include <ogl3d/math/omat4.h>

// Application-side representation of the constant buffer in the shader.
// Structure padding should be considered for more complex data types.
struct UniformData
{
	OMat4 world;
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
	// Cube definition using 36 vertices (6 faces × 2 triangles × 3 vertices).
	// Vertex layout: Position (3f) + Color (3f) — gradient on Y (dark red base)
	// Brightness = 0.6 when y=-0.5, 1.0 when y=0.5
	// Base color = (0.8, 0.1, 0.1)
	const f32 polygonVertices[] = {
		// Front face
		-0.5f, -0.5f,  0.5f,  0.48f, 0.06f, 0.06f,
		 0.5f, -0.5f,  0.5f,  0.48f, 0.06f, 0.06f,
		 0.5f,  0.5f,  0.5f,  0.80f, 0.10f, 0.10f,
		-0.5f, -0.5f,  0.5f,  0.48f, 0.06f, 0.06f,
		 0.5f,  0.5f,  0.5f,  0.80f, 0.10f, 0.10f,
		-0.5f,  0.5f,  0.5f,  0.80f, 0.10f, 0.10f,

		// Back face
		 0.5f, -0.5f, -0.5f,  0.48f, 0.06f, 0.06f,
		-0.5f, -0.5f, -0.5f,  0.48f, 0.06f, 0.06f,
		-0.5f,  0.5f, -0.5f,  0.80f, 0.10f, 0.10f,
		 0.5f, -0.5f, -0.5f,  0.48f, 0.06f, 0.06f,
		-0.5f,  0.5f, -0.5f,  0.80f, 0.10f, 0.10f,
		 0.5f,  0.5f, -0.5f,  0.80f, 0.10f, 0.10f,

		// Right face
		 0.5f, -0.5f,  0.5f,  0.48f, 0.06f, 0.06f,
		 0.5f, -0.5f, -0.5f,  0.48f, 0.06f, 0.06f,
		 0.5f,  0.5f, -0.5f,  0.80f, 0.10f, 0.10f,
		 0.5f, -0.5f,  0.5f,  0.48f, 0.06f, 0.06f,
		 0.5f,  0.5f, -0.5f,  0.80f, 0.10f, 0.10f,
		 0.5f,  0.5f,  0.5f,  0.80f, 0.10f, 0.10f,

		// Left face
		-0.5f, -0.5f, -0.5f,  0.48f, 0.06f, 0.06f,
		-0.5f, -0.5f,  0.5f,  0.48f, 0.06f, 0.06f,
		-0.5f,  0.5f,  0.5f,  0.80f, 0.10f, 0.10f,
		-0.5f, -0.5f, -0.5f,  0.48f, 0.06f, 0.06f,
		-0.5f,  0.5f,  0.5f,  0.80f, 0.10f, 0.10f,
		-0.5f,  0.5f, -0.5f,  0.80f, 0.10f, 0.10f,

		// Top face
		-0.5f,  0.5f,  0.5f,  0.80f, 0.10f, 0.10f,
		 0.5f,  0.5f,  0.5f,  0.80f, 0.10f, 0.10f,
		 0.5f,  0.5f, -0.5f,  0.80f, 0.10f, 0.10f,
		-0.5f,  0.5f,  0.5f,  0.80f, 0.10f, 0.10f,
		 0.5f,  0.5f, -0.5f,  0.80f, 0.10f, 0.10f,
		-0.5f,  0.5f, -0.5f,  0.80f, 0.10f, 0.10f,

		// Bottom face
		-0.5f, -0.5f, -0.5f,  0.48f, 0.06f, 0.06f,
		 0.5f, -0.5f, -0.5f,  0.48f, 0.06f, 0.06f,
		 0.5f, -0.5f,  0.5f,  0.48f, 0.06f, 0.06f,
		-0.5f, -0.5f, -0.5f,  0.48f, 0.06f, 0.06f,
		 0.5f, -0.5f,  0.5f,  0.48f, 0.06f, 0.06f,
		-0.5f, -0.5f,  0.5f,  0.48f, 0.06f, 0.06f
	};

	OVertexAttribute attribsList[] = {
		{3}, // position attribute slot
		{3}  // color attribute slot
	};

	m_polygonVAO = m_graphicsEngine->createVertexArrayObject({
		(void*)polygonVertices,
		sizeof(f32) * 6, // Stride: bytes per vertex
		36,               // Total vertices (cube)
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

	// Enable depth test and face culling for correct 3D rendering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
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

	// Use a dedicated time accumulator to drive animation so translations oscillate
	m_time += deltaTime;

	// Parameters for animation (tweak to taste)
	const f32 scaleSpeed = 2.0f;       // how fast the pulsing occurs
	const f32 translationSpeed = 1.5f; // how fast the object moves left-right
	const f32 translationAmp = 0.5f;   // amplitude of translation in world units

	// Compute scale (pulses between 0.5 and 1.0)
	auto currentScale = 0.75f + 0.25f * sinf(m_time * scaleSpeed);

	// Compute translation (oscillates between -translationAmp..+translationAmp)
	auto translationX = sinf(m_time * translationSpeed) * translationAmp;

	

	OMat4 world, temp;

	// Apply scale, rotation and translation (model matrix = Identity * Scale * Rotation * Translation)
	temp.setIdentity();
	temp.setScale(Ovec4(currentScale, currentScale, currentScale, 1));
	world *= temp;

	// Rotation angles for each axis (radians)
	const f32 rotSpeedX = 1.0f;
	const f32 rotSpeedY = 0.7f;
	const f32 rotSpeedZ = 1.3f;
	auto rotX = m_time * rotSpeedX;
	auto rotY = m_time * rotSpeedY;
	auto rotZ = m_time * rotSpeedZ;

	temp.setIdentity();
	temp.setRotationEuler(Ovec4(rotX, rotY, rotZ, 0));
	world *= temp;

	temp.setIdentity();
	temp.setTranslation(Ovec4(translationX, 0, 0, 1));
	world *= temp;

	// Update UBO content with the new calculation before drawing
	UniformData data = { world };
	m_uniform->setData(&data);

	m_graphicsEngine->setShaderProgram(m_shader);
	m_graphicsEngine->setVertexArrayObject(m_polygonVAO);

	// Pipeline Synchronization:
	// The Uniform Buffer must be bound to slot 0 to match the setUniformBufferSlot call above.
	// Use TriangleList to render the cube (GL_TRIANGLES).
	m_graphicsEngine->setUniformBuffer(m_uniform, 0);
	m_graphicsEngine->drawTriangles(TriangleList, m_polygonVAO->getVertexBufferSize(), 0);
}

void OGame::onQuit()
{
}

void OGame::quit()
{
	m_isRunning = false;
}