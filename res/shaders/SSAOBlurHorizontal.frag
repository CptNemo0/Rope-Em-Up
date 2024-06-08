#version 330 core
layout (location = 1) out vec3 color_texture;

in vec2 if_uv;

uniform sampler2D intermediate_texture;
const float div = 0.04166666 * 1.25;

void main()
{
    float w[7] = float[7](1, 2, 3, 4, 3, 2, 1);
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