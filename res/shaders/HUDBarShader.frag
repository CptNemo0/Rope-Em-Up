#version 330 core

in vec2 ov_texture;
out vec4 of_color;

uniform sampler2D u_texture;
uniform vec4 color;

uniform float percentage;

void main()
{
    vec4 new_color =  ov_texture.x < percentage ? color : vec4(1.0, 1.0, 1.0, 0.0);
    of_color = texture(u_texture, ov_texture) * new_color;
}