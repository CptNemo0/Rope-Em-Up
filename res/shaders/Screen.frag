#version 430
in vec2 uv;

uniform sampler2D texture_;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture_, uv);
}