#include <ogl3d/graphics/ouniformbuffer.h>
#include <glad/glad.h>

OUniformBuffer::OUniformBuffer(const OUniformBufferDesc& desc)
{
	// Generate and initialize the Uniform Buffer Object
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);

	// Pre-allocate storage on the GPU with the provided size descriptor
	glBufferData(GL_UNIFORM_BUFFER, desc.size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	m_size = desc.size;
}

OUniformBuffer::~OUniformBuffer()
{
	// Ensure memory release on cleanup
	glDeleteBuffers(1, &m_id);
}

void OUniformBuffer::setData(void* data)
{
	// Update the existing buffer memory with new data using sub-allocation
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

ui32 OUniformBuffer::getId()
{
	return m_id;
}