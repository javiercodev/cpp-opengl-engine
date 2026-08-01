#include <ogl3d/graphics/oshaderprogram.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <Windows.h>

namespace fs = std::filesystem;

// Absolute path to the running .exe's folder — used as a fallback base
// when a shader path isn't found relative to the current working directory.
static fs::path getExecutableDir()
{
	wchar_t path[MAX_PATH] = {};
	GetModuleFileNameW(NULL, path, MAX_PATH);
	return fs::path(path).parent_path();
}

OShaderProgram::OShaderProgram(const OShaderProgramDesc& desc)
{
	m_programId = glCreateProgram();
	attach(desc.vertexShaderFilePath, VertexShader);
	attach(desc.fragmentShaderFilePath, FragmentShader);
	link();
}

OShaderProgram::~OShaderProgram()
{
	// Detach + delete each stage that was successfully attached
	// (a stage that failed to load leaves its slot at 0, so it's skipped)
	for (ui32 i = 0; i < 2; i++)
	{
		if (m_attachedShaders[i])
		{
			glDetachShader(m_programId, m_attachedShaders[i]);
			glDeleteShader(m_attachedShaders[i]);
		}
	}
	glDeleteProgram(m_programId);
}

ui32 OShaderProgram::getId()
{
	return m_programId;
}

void OShaderProgram::attach(const wchar_t* shaderFilePath, const OShaderType& type)
{
	// Try the path as-is first (relative to the working directory); if that
	// doesn't exist, fall back to resolving it relative to the .exe's folder.
	// This lets the same relative path work whether the game is launched from
	// Visual Studio (working dir = project folder) or by double-clicking the .exe.
	fs::path fullPath = shaderFilePath;
	if (!fs::exists(fullPath))
	{
		fullPath = getExecutableDir() / shaderFilePath;
	}

	std::string shaderCode;
	std::ifstream shaderStream(fullPath);
	if (shaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
	}
	else
	{
		std::cerr << "ERROR: Could not open shader file" << std::endl;
		std::cerr << "  Tried: " << fullPath.string() << std::endl;
		return; // leaves this stage's slot at 0 / unattached
	}

	ui32 shaderId = 0;
	if (type == VertexShader)
		shaderId = glCreateShader(GL_VERTEX_SHADER);
	else if (type == FragmentShader)
		shaderId = glCreateShader(GL_FRAGMENT_SHADER);

	const char* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, NULL);
	glCompileShader(shaderId);

	int success = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512] = {};
		glGetShaderInfoLog(shaderId, 511, NULL, infoLog);
		std::cerr << "Shader compile error (";
		std::cerr << (type == VertexShader ? "vertex" : "fragment");
		std::cerr << "):\n" << infoLog << std::endl;
	}

	glAttachShader(m_programId, shaderId);
	m_attachedShaders[type] = shaderId;
}

void OShaderProgram::link()
{
	glLinkProgram(m_programId);

	int success = 0;
	glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512] = {};
		glGetProgramInfoLog(m_programId, 511, NULL, infoLog);
		std::cerr << "Shader link error:\n" << infoLog << std::endl;
	}
}