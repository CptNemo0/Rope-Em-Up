#version 330 core

struct PointLight
{
    float intensity;
    vec3 position;
	vec3 ambient_colour;
	vec3 diffuse_colour;
	vec3 specular_colour;
};

in vec3 if_fragment_position;
in vec3 if_normal;
in vec2 if_texture;

uniform vec3 camera_position;
uniform sampler2D this_texture;
uniform PointLight light;
    
int steps = 100;
float sum = 0.0f;
vec3 P    = ...;
vec3 Wo   = ...;
vec3 N    = ...;
float dW  = 1.0f / steps;
for(int i = 0; i < steps; ++i) 
{
    vec3 Wi = getNextIncomingLightDir(i);
    sum += Fr(P, Wi, Wo) * L(P, Wi) * dot(N, Wi) * dW;
}

