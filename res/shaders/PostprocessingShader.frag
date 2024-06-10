#version 330 core
in vec2 if_uv;

out vec4 FragColor;

uniform sampler2D color_texture;
uniform sampler2D bloom_texture;

uniform bool bloom;
uniform bool slowed_time;

uniform vec3 cbg;

uniform float if_time;

uniform float vignete_amount;
uniform float vignete_contrast;
uniform float noise_amount;
uniform float transition_vignette_amount;
uniform vec2 resolution;

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
    float vig = uv.x * uv.y * vignete_contrast;
    vig = pow(vig, vignete_amount);

    return color * vig;
}

vec3 apply_film_grain(vec3 color)
{
    vec2 uv = if_uv + (sin(if_time) + 2.0);
    float noise = (fract(sin(dot(uv, vec2(12.9898,78.233) * 2.0)) * 43758.5453)) ;
    return color - noise * noise_amount;
}

vec3 apply_vignette(vec3 color, float value)
{
    vec2 position = if_uv - 0.5;
    float dist = length(position * vec2(resolution.y / resolution.x, 1.0));

    float radius = value;
    float softness = 0.2;
    float vignette = smoothstep(radius, radius - softness, dist);

    color.rgb = color.rgb - (1.0 - vignette);

    return color;
}

vec3 bloor_bloom()
{
    float div = 0.04166666 * 0.1875;
    float w[7] = float[7](1, 2, 3, 4, 3, 2, 1);

    int idx_x = 0;
    int idx_y = 0;

    vec2 texel_size = 1.0 / vec2(textureSize(bloom_texture, 0));
    vec3 result = vec3(0.0f);
    for (int x = -3; x < 3; ++x) 
    {
        for (int y = -3; y < 3; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texel_size;
            result += w[idx_x] * w[idx_y] * texture(bloom_texture, if_uv + offset).rgb;
            idx_y++;
        }
        idx_x++;
        idx_y = 0;
    }
    result *= div;
    return result;
}

void main()
{
    vec3 color = vec3(texture(color_texture, if_uv));
    color = adjust_contrast(color, cbg.x);
    color = adjust_brightness(color, cbg.y);
    color = adjust_gamma(color, cbg.z);
    color = apply_film_grain(color);

    if (bloom)
    { 
        color = color + bloor_bloom();
    }

    // prosze zostawic to jako ostatnie
    color = apply_vignete(color);
    color = vec3(clamp(color, 0.0, 1.0));
    color = apply_vignette(color, transition_vignette_amount);

    FragColor = vec4(color.rgb, 1.0);
} 