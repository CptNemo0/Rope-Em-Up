#version 330 core

layout (location = 1) out vec3 albedo_texture;
layout (location = 6) out vec3 mask_texture;

//out vec4 FragColor;
in vec3 World_position;

uniform samplerCube environmentMap;

void main()
{		
    vec3 envColor = texture(environmentMap, World_position).rgb;
    
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
    
    albedo_texture = envColor;
    mask_texture = vec3(0.0);
}