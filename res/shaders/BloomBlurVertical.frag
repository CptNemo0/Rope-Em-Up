#version 330 core
layout (location = 2) out vec3 vecrtical_texture;

in vec2 if_uv;

uniform sampler2D horizontal_texture;

const float div = 0.11111111111;

void main()
{
    vec2 texel_size = 1.0 / vec2(textureSize(horizontal_texture, 0));
    vec3 result = vec3(0.0);
    for (int x = -4; x < 4; x++) 
    {
        vec2 offset = vec2(0.0, x) * texel_size;
        result += texture(horizontal_texture, if_uv + offset).rgb;
    }
    result *= div;
    vecrtical_texture = result;
}