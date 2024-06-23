#version 330 core

layout (location = 0) in vec3 iv_pos;

uniform mat4 view_matrix;

void main()
{
    gl_Position = view_matrix * vec4(iv_pos, 1.0);
}