#version 330 core
layout (location = 0) out vec3 intermediate_texture;

in vec2 if_uv;

uniform sampler2D ssao_texture;

const float div = 0.1;

void main()
{
    float w[7] = float[7](0.5, 1, 2, 4, 2, 1, 0.5);
    vec2 texel_size = 1.0 / vec2(textureSize(ssao_texture, 0));
    float result = 0.0;
    int idx = 0;
    for (float x = -3.0; x < 3.1; x+=1.0) 
    {
        vec2 offset = vec2(x, 0.0) * texel_size;
        result += w[idx] * texture(ssao_texture, if_uv + offset).r;
        idx++;
    }
    result *= div;
    intermediate_texture = vec3(result);
}