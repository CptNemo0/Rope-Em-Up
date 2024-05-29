#version 330 core
layout (location = 0) in vec3 iv_position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

out vec3 World_position;

void main()
{
    World_position = iv_position;

	mat4 rotView = mat4(mat3(view_matrix));
	vec4 clipPos = projection_matrix * rotView * vec4(World_position, 1.0);

	gl_Position = clipPos.xyww;
}