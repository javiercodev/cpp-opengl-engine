#include <ogl3d/graphics/overtexarrayobject.h>
#include <glad/glad.h>

OVertexArrayObject::OVertexArrayObject(const OVertexBufferDesc& data)
{
	if (!data.listSize) OGL3D_ERROR("OVertexArrayObject | listSize is NULL");
	if (!data.vertexSize) OGL3D_ERROR("OVertexArrayObject | vertexSize is NULL");
	if (!data.verticesList) OGL3D_ERROR("OVertexArrayObject | verticesList is NULL");

	// Keep a copy of the incoming descriptor to maintain state 
	// for getter functions (matches your current header).
	m_vertexBufferData = data;

	// 1. Create and bind VAO first (required in Core Profile)
	glGenVertexArrays(1, &m_vertexArrayObjectId);
	glBindVertexArray(m_vertexArrayObjectId);

	// 2. Create VBO, bind it and upload vertex data to GPU memory
	glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, data.vertexSize * data.listSize, data.verticesList, GL_STATIC_DRAW);

	// 3. Define vertex layout by describing each attribute (Position, Color, etc.)
	// The offset increments based on the number of elements per attribute.
	ui32 offset = 0;
	for (ui32 i = 0; i < data.attributesListSize; ++i)
	{
		glVertexAttribPointer(
			i,
			data.attributes[i].numElements,
			GL_FLOAT,
			GL_FALSE,
			data.vertexSize,
			(void*)(offset * sizeof(f32))
		);
		glEnableVertexAttribArray(i);

		offset += data.attributes[i].numElements;
	}

	// 4. Unbind VAO (Good practice to avoid unintended state changes)
	glBindVertexArray(0);
}

OVertexArrayObject::~OVertexArrayObject()
{
	// Clean up buffer and array objects on destruction
	glDeleteBuffers(1, &m_vertexBufferID);
	glDeleteVertexArrays(1, &m_vertexArrayObjectId);
}

ui32 OVertexArrayObject::getId()
{
	return m_vertexArrayObjectId;
}

ui32 OVertexArrayObject::getVertexBufferSize()
{
	return m_vertexBufferData.listSize;
}

ui32 OVertexArrayObject::getVertexSize()
{
	return m_vertexBufferData.vertexSize;
}