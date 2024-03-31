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

float DistributionGGX(vec3 N, vec3 H, float a) //funkcja rozk³adu wektorów normalnych Trowbridge-Reitz GGX
    {
        float a2     = a*a;
        float NdotH  = max(dot(N, H), 0.0);
        float NdotH2 = NdotH*NdotH;

        float nom    = a2;
        float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
        denom        = PI * denom * denom;

        return nom / denom;
    }

    float GeometrySchlickGGX(float NdotV, float k)
    {
        float nom   = NdotV;
        float denom = NdotV * (1.0 - k) + k;

        return nom / denom;
    }

    float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
    {
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float ggx1 = GeometrySchlickGGX(NdotV, k);
        float ggx2 = GeometrySchlickGGX(NdotL, k);

        return ggx1 * ggx2;
    }

vec3 F0 = vec3(0.04);
F0      = mix(F0, surfaceColor.rgb, metalness);
float cosTheta = dot(N, v); //v - kierunek patrzenia

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
