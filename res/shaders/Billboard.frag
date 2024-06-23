#version 430 core

in vec2 uv;

uniform sampler2D albedo;
uniform vec4 color;

out vec4 of_color;

void main()
{
    of_color = texture(albedo, uv);
}