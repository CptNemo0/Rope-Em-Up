#version 330 core

in vec2 ov_texcoords;
out vec4 of_color;

uniform sampler2D tex;
uniform vec3 tex_color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, ov_texcoords).r);
    of_color = vec4(tex_color, 1.0) * sampled;
}  