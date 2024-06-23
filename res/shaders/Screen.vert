#version 430 core
layout (location = 0) in vec2 iv_position;

out vec2 uv;

void main()
{
    uv = iv_position * 0.5 + 0.5;
    gl_Position = vec4(iv_position.x, iv_position.y, 0.0, 1.0); 
}  