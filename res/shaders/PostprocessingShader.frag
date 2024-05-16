#version 330 core
in vec2 if_uv;

out vec4 FragColor;

uniform sampler2D color_texture;

uniform vec3 cbg;

uniform float if_time;

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

vec3 apply_vignete(vec3 color)
{
    vec2 uv = if_uv * (1.0 - if_uv);
    float vig = uv.x * uv.y * 50.0f;
    vig = pow(vig, 0.25);

    return color * vig;
}

vec3 apply_film_grain(vec3 color)
{
    float noise_amount = 0.025 * (sin(if_time * 1.25) + 2.0);
    float noise = (fract(sin(dot(if_uv, vec2(12.9898,78.233)*2.0)) * 43758.5453)) ;
    return color - noise * noise_amount;
}

void main()
{
    vec3 color = vec3(texture(color_texture, if_uv));
    color = adjust_contrast(color, cbg.x);
    color = adjust_brightness(color, cbg.y);
    color = adjust_gamma(color, cbg.z);
    color = apply_film_grain(color);
    color = apply_vignete(color);
    FragColor = vec4(color, 1.0);
} 