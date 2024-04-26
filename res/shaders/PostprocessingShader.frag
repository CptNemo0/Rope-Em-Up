#version 330 core
in vec2 if_uv;

out vec4 FragColor;

uniform sampler2D screen_texture;

void main()
{
    vec3 col = texture(screen_texture, if_uv).rgb;
    col += vec3(0.0, 0.1, 0.0);
    FragColor = vec4(col, 1.0);
} 