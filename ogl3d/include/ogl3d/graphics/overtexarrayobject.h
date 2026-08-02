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

	// NOTE: declared but not yet implemented in overtexarrayobject.cpp,
	// and m_vertexBufferData is never assigned in the constructor either —
	// calling these will fail to link until both are wired up.
	ui32 getVertexBufferSize();
	ui32 getVertexSize();

private:
	ui32 m_vertexBufferID = 0;
	ui32 m_vertexArrayObjectId = 0;
	OVertexBufferDesc m_vertexBufferData;
};