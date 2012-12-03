#version 330

in vec2 finalTexCoord;

uniform vec4 color;
uniform sampler2D _sampler;

out vec4 outputColor;

void main()
{
	outputColor = texture2D(_sampler, finalTexCoord).rgba;
}