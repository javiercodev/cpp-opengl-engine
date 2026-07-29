#include <ogl3d/game/ogame.h>
#include <ogl3d/window/owindow.h>
#include <ogl3d/graphics/ographicsengine.h>
#include <glad/glad.h>
#include <iostream>

// --- Shader helpers (local to this file, not part of the public engine API yet) ---

// Compiles a single shader stage and prints the GL error log if it fails.
static ui32 compileShader(const char* source, ui32 type)
{
	ui32 id = glCreateShader(type);
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	int success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(id, 511, nullptr, infoLog);
		std::cerr << "Shader compile error:\n" << infoLog << std::endl;
	}
	return id;
}

// Compiles + links a vertex/fragment pair into a usable program.
// Shader objects are deleted after linking; only the linked program is kept.
static ui32 createShaderProgram(const char* vertSrc, const char* fragSrc)
{
	ui32 vs = compileShader(vertSrc, GL_VERTEX_SHADER);
	ui32 fs = compileShader(fragSrc, GL_FRAGMENT_SHADER);
	ui32 prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);

	int success;
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(prog, 511, nullptr, infoLog);
		std::cerr << "Shader link error:\n" << infoLog << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
	return prog;
}

// Demo shader program ID. Kept as a file-local static instead of an OGame
// member for now, since there's no shader-management class yet.
static ui32 s_shaderProgram = 0;

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

	// Minimal pass-through shaders: just place the vertices and paint them orange
	const char* vertSrc = R"(
		#version 460 core
		layout (location = 0) in vec3 aPos;
		void main()
		{
			gl_Position = vec4(aPos, 1.0);
		}
	)";

	const char* fragSrc = R"(
		#version 460 core
		out vec4 FragColor;
		void main()
		{
			FragColor = vec4(1.0, 0.5, 0.2, 1.0);
		}
	)";

	s_shaderProgram = createShaderProgram(vertSrc, fragSrc);
}

void OGame::onUpdate()
{
	glUseProgram(s_shaderProgram);

	m_graphicsEngine->setVertexArrayObject(m_triangleVAO);
	m_graphicsEngine->drawTriangles(3, 0);
}

void OGame::onQuit()
{
	glDeleteProgram(s_shaderProgram);
	s_shaderProgram = 0;
}

void OGame::quit()
{
	m_isRunning = false;
}