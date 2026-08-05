#pragma once
#include <memory>
#include <sstream>
#include <iostream>
#include <stdexcept>

// Forward declaration + smart pointer alias: lets headers pass around a
// vertex array object, a uniform buffer or a shader program without including their real
// headers (overtexarrayobject.h, ouniformbuffer.h, oshaderprogram.h — and glad.h transitively)
class OVertexArrayObject;
class OUniformBuffer; // Added forward declaration for UBO support
class OShaderProgram;

typedef std::shared_ptr<OVertexArrayObject> OVertexArrayObjectPtr;
typedef std::shared_ptr<OUniformBuffer> OUniformBufferPtr; // Smart pointer alias for easier buffer management
typedef std::shared_ptr<OShaderProgram> OShaderProgramPtr;

// Fixed-width-style type aliases used across the engine (kept short and explicit)
typedef float f32;
typedef int i32;
typedef unsigned int ui32;

// Describes a single vertex attribute (e.g. position, color, uv) in terms
// of how many floats it takes up. Used to build the attributesList passed
// to OVertexBufferDesc.
struct OVertexAttribute
{
	ui32 numElements = 0;
};

// Raw vertex data + layout info, passed to OGraphicsEngine::createVertexArrayObject.
// vertexSize is the byte size of a single vertex (used as the GL stride),
// listSize is how many vertices are in the buffer.
// attributes/attributesListSize describe how that vertex is split into
// attribute slots (see OVertexArrayObject's constructor for how offsets
// are derived from this list).
struct OVertexBufferDesc
{
	void* verticesList = nullptr;
	ui32 vertexSize = 0;
	ui32 listSize = 0;

	OVertexAttribute* attributes = nullptr;
	ui32 attributesListSize = 0;
};

// File paths for a vertex/fragment shader pair, passed to
// OGraphicsEngine::createShaderProgram. Paths can be relative to the
// working directory or to the executable (see OShaderProgram::attach).
struct OShaderProgramDesc
{
	const wchar_t* vertexShaderFilePath;
	const wchar_t* fragmentShaderFilePath;
};

// Describes the byte size requirement for a new Uniform Buffer Object.
struct OUniformBufferDesc
{
	ui32 size = 0;
};

// Specifies the primitive topology to be used in draw commands.
enum OtriangleType
{
	TriangleList = 0, // GL_TRIANGLES
	TriangleStrip     // GL_TRIANGLE_STRIP
};

// Indexes into OShaderProgram::m_attachedShaders — values must stay 0/1
enum OShaderType
{
	VertexShader = 0,
	FragmentShader
};

// --- Minimal engine-wide logging/error macros ---
// OGL3D_ERROR throws (stops execution); OGL3D_WARNING/OGL3D_INFO just log.
// All three accept a chain of "<<"-streamable values, e.g.:
//   OGL3D_WARNING("Shader " << path << " not found");

#define OGL3D_ERROR(message)\
{\
std::stringstream m;\
m<< "OGL3D Error: " << message << std::endl;\
throw std::runtime_error(m.str());\
}

#define OGL3D_WARNING(message)\
std::wclog << "OGL3D Warning: " << message << std::endl;

#define OGL3D_INFO(message)\
std::wclog << "OGL3D Info: " << message << std::endl;