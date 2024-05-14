#version 330 core
layout (location = 0) out vec3 color_texture;

in vec2 if_uv;

uniform sampler2D ssao_texture;

const float div = 0.0625;

void main()
{
    vec2 texel_size = 1.0 / vec2(textureSize(ssao_texture, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texel_size;
            result += texture(ssao_texture, if_uv + offset).r;
        }
    }
    result *= div;
    color_texture = vec3(result);
}