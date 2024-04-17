#version 330 core
layout (location = 0) in vec3 iv_position;

out vec3 World_position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main()
{
    World_position = iv_position;
    gl_Position =  projection_matrix * view_matrix * vec4(World_position, 1.0);
}