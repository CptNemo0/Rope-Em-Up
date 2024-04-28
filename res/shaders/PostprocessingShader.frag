#version 330 core
in vec2 if_uv;

out vec4 FragColor;

uniform sampler2D screen_texture;

uniform vec3 cbg;

vec3 adjust_contrast(vec3 color, float value) 
{
    return ((color - 0.5) * value) + 0.5;
}

vec3 adjust_brightness(vec3 color, float value) 
{
    return color + value;
}

vec3 adjust_gamma(vec3 color, float value) 
{
    return pow(color, vec3(1.0 / value));
}

void main()
{
    vec3 color = vec3(texture(screen_texture, if_uv));
    color = adjust_contrast(color, cbg.x);
    color = adjust_brightness(color, cbg.y);
    color = adjust_gamma(color, cbg.z);
    FragColor = vec4(color, 1.0);
} 