#version 330 core
layout (location = 0) out vec3 color_texture;

uniform sampler2D position_texture;
uniform sampler2D albedo_texture;
uniform sampler2D normal_texture;
uniform sampler2D mra_texture;
uniform sampler2D ssao_texture;

uniform vec3 camera_position;

const int light_num = 1;
uniform vec3 light_positions[light_num];
uniform vec3 light_colors[light_num];

in vec2 if_uv;

uniform samplerCube irradianceMap;

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) //funkcja rozk³adu wektorów normalnych Trowbridge-Reitz GGX
{
		float a      = roughness * roughness;
        float a2     = a*a;
        float NdotH  = max(dot(N, H), 0.0);
        float NdotH2 = NdotH*NdotH;

        float nom    = a2;
        float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
        denom        = PI * denom * denom;

        return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
        float r = (roughness + 1.0);
        float k = (r*r) / 8.0;

        float num   = NdotV;
        float denom = NdotV * (1.0 - k) + k;

        return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float ggx2  = GeometrySchlickGGX(NdotV, roughness);
        float ggx1  = GeometrySchlickGGX(NdotL, roughness);

        return ggx1 * ggx2;
}

vec3 getNormalFromMap(vec3 position)
{
    vec3 tangentNormal = texture(normal_texture, if_uv).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(position);
    vec3 Q2  = dFdy(position);
    vec2 st1 = dFdx(if_uv);
    vec2 st2 = dFdy(if_uv);

    vec3 N   = normalize(tangentNormal * 2.0 - 1.0);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    vec3 position = texture(position_texture, if_uv).rgb;
    vec3 albedo = texture(albedo_texture, if_uv).rgb;
    vec3 mra = texture(mra_texture, if_uv).rgb;
	float metallic = mra.r;
	float roughness = mra.g;
	float ao = mra.b;
    float ssao = texture(ssao_texture, if_uv).r;

    vec3 N = normalize(texture(normal_texture, if_uv).rgb * 2.0 - 1.0);
    vec3 V = normalize(-position);  

    vec3 ambient = vec3(0.15 * albedo * ssao);
    vec3 lighting = vec3(0.0, 0.0, 0.0);

    
    for(int i = 0 ; i < light_num ; i++)
    {
        vec3 light_dir = normalize(light_positions[i] - position);
        vec3 diffuse = max(dot(N, light_dir), 0.0) * albedo * light_colors[i];

        vec3 H = normalize(light_dir + V);  
        float spec = pow(max(dot(N, H), 0.0), 8.0);
        vec3 specular = light_colors[i] * spec;

        float dist = length(light_positions[i] - position);
        float attenuation = 1.0 / (1.0 + dist + dist * dist);

        diffuse *= attenuation;
        specular *= attenuation;

        lighting += diffuse + specular;
    }

    vec3 color = ambient + lighting / light_num;
    color = pow(color, vec3(1.0/2.2));
    color_texture = color;
}