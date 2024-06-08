#version 330 core
layout (location = 1) out vec3 color_texture;

in vec2 if_uv;

uniform sampler2D intermediate_texture;

const float div = 0.1;

void main()
{
    float w[7] = float[7](0.5, 1, 2, 4, 2, 1, 0.5);
    vec2 texel_size = 1.0 / vec2(textureSize(intermediate_texture, 0));
    float result = 0.0;
    int idx = 0;
    for (float x = -3.0; x < 3.01; x+=1.0) 
    {
        vec2 offset = vec2(0.0, x) * texel_size;
        result += w[idx] * texture(intermediate_texture, if_uv + offset).r;
        idx++;
    }
    result *= div;
    color_texture = vec3(result);
}