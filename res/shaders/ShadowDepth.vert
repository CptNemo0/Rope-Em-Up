#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model_matrix;
uniform mat4 light_space_matrix;

void main()
{
	gl_Position = light_space_matrix * model_matrix * vec4(position, 1.0);
}