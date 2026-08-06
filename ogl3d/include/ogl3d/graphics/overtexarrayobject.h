#pragma once
#include <ogl3d/oprerequisites.h>

// Wraps a GL Vertex Array Object together with the Vertex Buffer Object
// that feeds it. Construction uploads the data to the GPU immediately;
// destruction frees both GL objects.
class OVertexArrayObject
{
public:
	OVertexArrayObject(const OVertexBufferDesc& data);
	~OVertexArrayObject();

	ui32 getId();

	// Getters return the number of vertices in the buffer and the stride
	// (vertex size in bytes). These are implemented in overtexarrayobject.cpp
	// and the constructor stores a copy of the incoming OVertexBufferDesc so
	// the values remain valid for the lifetime of the VAO object.

	ui32 getVertexBufferSize();
	ui32 getVertexSize();

private:
	ui32 m_vertexBufferID = 0;
	ui32 m_vertexArrayObjectId = 0;
	OVertexBufferDesc m_vertexBufferData;
};