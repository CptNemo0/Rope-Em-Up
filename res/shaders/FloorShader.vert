#version 430 core

layout (location = 0) in vec3 iv_position;
layout (location = 1) in vec2 iv_uv;

out vec3 world_position;
out vec2 uv;

uniform vec2 position;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    mat4 translation_matrix = mat4(1.0);
    translation_matrix[3][0] = position.x;
    translation_matrix[3][1] = 1.0;
    translation_matrix[3][2] = position.y;
    translation_matrix[3][3] = 1.0;

    gl_Position = projection_matrix * view_matrix * translation_matrix * vec4(iv_position, 1.0);

    world_position = vec3(position.x, 1.0, position.y);
    uv = iv_uv;
}