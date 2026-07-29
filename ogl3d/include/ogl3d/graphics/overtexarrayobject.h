#pragma once
#include <ogl3d/oprerequisites.h>

// Wraps a GL Vertex Array Object together with the Vertex Buffer Object
// that feeds it. Construction uploads the data to the GPU immediately;
// destruction frees both GL objects.
class OVertexArrayObject
{
public:
	OVertexArrayObject(const OVertexBufferData& data);
	~OVertexArrayObject();

	ui32 getId();

private:
	ui32 m_vertexBufferID = 0;
	ui32 m_vertexArrayObjectId = 0;
};