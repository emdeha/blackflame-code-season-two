#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 finalTexCoord;

void main()
{
	gl_Position = position;
	finalTexCoord = texCoord;
}