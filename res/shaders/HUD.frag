#version 330 core

in vec2 ov_texture;
out vec4 of_color;

uniform sampler2D u_texture;
uniform vec4 color;

void main()
{
    of_color = texture(u_texture, ov_texture) * color;
}