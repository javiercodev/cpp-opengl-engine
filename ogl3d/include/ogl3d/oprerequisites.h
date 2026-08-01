#pragma once
#include <memory>

// Forward declaration + smart pointer alias: lets headers pass around a
// vertex array object without including overtexarrayobject.h (and glad.h)
class OVertexArrayObject;
class OShaderProgram;

typedef std::shared_ptr<OVertexArrayObject> OVertexArrayObjectPtr;
typedef std::shared_ptr<OShaderProgram> OShaderProgramPtr;

// Fixed-width-style type aliases used across the engine (kept short and explicit)
typedef float f32;
typedef int i32;
typedef unsigned int ui32;

// Raw vertex data + layout info, passed to OGraphicsEngine::createVertexArrayObject.
// vertexSize is the byte size of a single vertex (used as the GL stride),
// listSize is how many vertices are in the buffer.
struct OVertexBufferData
{
	void* verticesList = nullptr;
	ui32 vertexSize = 0;
	ui32 listSize = 0;
};

// File paths for a vertex/fragment shader pair, passed to
// OGraphicsEngine::createShaderProgram. Paths can be relative to the
// working directory or to the executable (see OShaderProgram::attach).
struct OShaderProgramDesc
{
	const wchar_t* vertexShaderFilePath;
	const wchar_t* fragmentShaderFilePath;
};

// Indexes into OShaderProgram::m_attachedShaders — values must stay 0/1
enum OShaderType
{
	VertexShader = 0,
	FragmentShader
};