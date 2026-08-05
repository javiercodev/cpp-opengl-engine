#pragma once
#include <ogl3d/oprerequisites.h>

// Represents a hardware buffer (UBO) used to store constant data shared across shaders.
// Unlike standard uniforms, UBOs allow for larger data blocks and efficient reuse
// across different shader programs.
class OUniformBuffer
{
public:
	// Allocates the buffer on the GPU based on the size specified in OUniformBufferDesc.
	OUniformBuffer(const OUniformBufferDesc& desc);
	~OUniformBuffer();

	// Updates the entire buffer content with new data from the CPU.
	// Typically used to send frame-varying data like Transformation Matrices or Timers.
	void setData(void* data);

	// Returns the OpenGL handle for this buffer.
	ui32 getId();
private:
	ui32 m_id = 0;   // OpenGL Buffer Object ID
	ui32 m_size = 0; // Size in bytes, cached to ensure setData does not overflow
};