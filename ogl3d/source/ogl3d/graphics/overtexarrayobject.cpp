#include <ogl3d/graphics/overtexarrayobject.h>
#include <glad/glad.h>

OVertexArrayObject::OVertexArrayObject(const OVertexBufferData& data)
{
	// 1. Create and bind VAO first (required in Core Profile)
	glGenVertexArrays(1, &m_vertexArrayObjectId);
	glBindVertexArray(m_vertexArrayObjectId);

	// 2. Create VBO, bind it and upload data
	glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, data.vertexSize * data.listSize, data.verticesList, GL_STATIC_DRAW);

	// 3. Describe vertex layout: location 0, 3 floats per position, stride = vertexSize
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, data.vertexSize, (void*)0);
	glEnableVertexAttribArray(0);

	// 4. Unbind VAO (good practice)
	glBindVertexArray(0);
}

OVertexArrayObject::~OVertexArrayObject()
{
	glDeleteBuffers(1, &m_vertexBufferID);
	glDeleteVertexArrays(1, &m_vertexArrayObjectId);
}

ui32 OVertexArrayObject::getId()
{
	return m_vertexArrayObjectId;
}