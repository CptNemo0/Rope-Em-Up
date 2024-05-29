#version 330 core
layout (location = 0) in vec3 iv_position;
layout (location = 1) in vec2 iv_texture;

out vec2 TexCoords;

void main()
{
    TexCoords = iv_texture;
	gl_Position = vec4(iv_position, 1.0);
}