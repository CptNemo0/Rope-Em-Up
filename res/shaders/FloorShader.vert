#version 460 core
layout (location = 0) in vec3 iv_position;
layout (location = 1) in vec2 iv_uv;

out vec2 uv_tc;

void main()
{
    gl_Position = vec4(iv_position, 1.0);
    uv_tc = iv_uv;
}