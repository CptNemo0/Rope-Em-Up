#version 330 core
layout (location = 1) out vec3 horizontal_texture;

in vec2 if_uv;

uniform sampler2D downscaled_texture;

const float div = 0.1428571428;

void main()
{
    vec2 texel_size = 1.0 / vec2(textureSize(downscaled_texture, 0));
    vec3 result = vec3(0.0);
    //int idx = 0;
    for (float x = -2.0; x < 2.1; x+=1.0) 
    {
        vec2 offset = vec2(x, 0.0) * texel_size;
        result += texture(downscaled_texture, if_uv + offset).rgb;
        //idx++;
    }
    result *= div;
    horizontal_texture = result;
}