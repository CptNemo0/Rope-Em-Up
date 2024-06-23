#version 430
layout (location = 0) out vec4 id_;
in vec2 uv;

uniform sampler2D texture_;

out vec4 FragColor;

void main()
{
	id_ = texture(texture_, uv);
}