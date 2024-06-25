#version 330 core
out vec4 FragColor;

uniform float transparency;

void main()
{
    FragColor = vec4(0.768, 0.686, 0.149, transparency);
}