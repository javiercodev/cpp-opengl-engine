#version 410 core

layout (row_major) uniform UniformData
{
	mat4 world;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 vertOutColor;


void main()
{
	
	gl_Position = world * vec4(position, 1);

	vertOutColor = color + vec3(world[3].xyz) * 0.5;
}
