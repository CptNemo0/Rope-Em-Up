#version 330 core
layout (location = 0) in vec2 iv_position;

uniform mat4 model_matrix;

void main()
{
	gl_Position = model_matrix * vec4(iv_position.x, -iv_position.y, 0.0, 1.0);
}