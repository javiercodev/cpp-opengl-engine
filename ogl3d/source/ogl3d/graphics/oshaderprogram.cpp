#include <ogl3d/graphics/oshaderprogram.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <vector>

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
	// (a stage that failed to load/compile leaves its slot at 0, so it's skipped)
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
		OGL3D_WARNING("OShaderProgram | " << shaderFilePath << " not found");
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

	// Check the real compile status first. The info log is only used for
	// diagnostics here — some drivers write a non-empty log even on a
	// successful compile, so its presence alone can't be used to decide
	// success/failure (that was the previous bug: any non-empty log bailed
	// out before glAttachShader below, even for shaders that compiled fine).
	int success = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	i32 logLength = 0;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::vector<char> errorMessage(logLength + 1);
		glGetShaderInfoLog(shaderId, logLength, NULL, &errorMessage[0]);
		OGL3D_WARNING("OShaderProgram | " << shaderFilePath
			<< (success ? " compiled with warnings: " : " compile error: ")
			<< std::endl << &errorMessage[0]);
	}

	if (!success)
	{
		glDeleteShader(shaderId); // never attached, so it won't be cleaned up in the destructor
		return; // leaves this stage's slot at 0 / unattached
	}

	glAttachShader(m_programId, shaderId);
	m_attachedShaders[type] = shaderId;

	OGL3D_INFO("OShaderProgram | " << shaderFilePath << " compiled and attached successfully");
}

void OShaderProgram::link()
{
	glLinkProgram(m_programId);

	int success = 0;
	glGetProgramiv(m_programId, GL_LINK_STATUS, &success);

	i32 logLength = 0;
	glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::vector<char> errorMessage(logLength + 1);
		glGetProgramInfoLog(m_programId, logLength, NULL, &errorMessage[0]);
		OGL3D_WARNING("OShaderProgram | "
			<< (success ? "link warnings: " : "link error: ")
			<< std::endl << &errorMessage[0]);
	}
}