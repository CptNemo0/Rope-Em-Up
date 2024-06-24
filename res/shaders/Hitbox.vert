#version 430 core
layout (location = 0) in vec3 iv_position;
layout (location = 1) in vec2 iv_tex;

out vec3 world_position;
out vec2 uv;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	world_position = iv_position;
	uv = iv_tex;
	gl_Position = projection_matrix * view_matrix * vec4(iv_position, 1.0f);
}
