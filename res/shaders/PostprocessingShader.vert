#version 330 core
layout (location = 0) in vec2 iv_position;
layout (location = 1) in vec2 iv_uv;

out vec2 if_uv;

void main()
{
    if_uv = iv_uv;
    gl_Position = vec4(iv_position.x, iv_position.y, 0.0, 1.0); 
}  