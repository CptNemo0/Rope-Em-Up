#version 330 core
layout (location = 0) in vec2 iv_position;

out vec2 if_texture;

uniform mat4 model_matrix;

void main()
{
	gl_Position = model_matrix * vec4(iv_position, 0.0, 1.0);
	if_texture = vec2((iv_position.x + 1.0) / 2.0, 1.0 - (iv_position.y + 1.0) / 2.0);
}