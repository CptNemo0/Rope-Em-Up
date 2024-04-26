#version 330 core

layout (location = 0) in vec3 iv_pos;
layout (location = 1) in float iv_size;
layout (location = 2) in vec4 iv_color;
layout (location = 3) in float iv_rotation;

out OV
{
    float size;
    vec4 color;
    float rotation;
} ov;

uniform mat4 view_matrix;

void main()
{
    gl_Position = view_matrix * vec4(iv_pos, 1.0);
    ov.size = iv_size;
    ov.color = iv_color;
    ov.rotation = iv_rotation;
}