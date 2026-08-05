#pragma once
#include <ogl3d/oprerequisites.h>

// Owns a linked GL shader program built from a vertex + fragment shader file.
// Loading, compiling, and linking all happen in the constructor; the program
// is ready to use (via getId() + glUseProgram) as soon as construction returns.
class OShaderProgram
{
public:
	OShaderProgram(const OShaderProgramDesc& desc);
	~OShaderProgram();

	ui32 getId();

	// Logic for Uniform Buffer Objects (UBO):
	// Maps a uniform block name defined in the shader to a specific binding point (slot).
	// This must match the 'slot' used in OGraphicsEngine::setUniformBuffer to sync data.
	void setUniformBufferSlot(const char* name, ui32 slot);

private:
	// Reads, compiles, and attaches a single shader stage from disk
	void attach(const wchar_t* shaderFilePath, const OShaderType& type);
	void link();
private:
	ui32 m_programId = 0;
	ui32 m_attachedShaders[2] = {}; // indexed by OShaderType (Vertex=0, Fragment=1)
};