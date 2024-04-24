#version 330 core

in vec2 og_texcoord;
in vec4 og_color;

out vec4 of_color;

uniform sampler2D u_texture;

void main()
{
    of_color = texture(u_texture, og_texcoord) * og_color;
}