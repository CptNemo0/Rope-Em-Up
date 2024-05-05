#version 330 core
layout (location = 0) out vec3 color_texture;

//out vec4 FragColor;

uniform sampler2D position_texture;
uniform sampler2D albedo_texture;
uniform sampler2D normal_texture;
uniform sampler2D mra_texture;
uniform sampler2D ssao_texture;

uniform vec3 camera_position;

const int light_num = 3;
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

void main()
{
    vec3 World_position = texture(position_texture, if_uv).rgb;
    vec3 albedo = texture(albedo_texture, if_uv).rgb;
    vec3 mra = texture(mra_texture, if_uv).rgb;
	float metallic = mra.r;
	float roughness = mra.g;
	float ao = mra.b;
    float ssao = texture(ssao_texture, if_uv).r;

    vec3 N = normalize(texture(normal_texture, if_uv).rgb);
    vec3 V = normalize(camera_position - World_position);
    
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    //The reflectance equation
	vec3 Lo = vec3(0.0);
    for(int i = 0; i < light_num; ++i) 
    {
        //radiation
        vec3 L = normalize(light_positions[i] - World_position);
        vec3 H = normalize(V + L);

        float distance    = length(light_positions[i] - World_position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = light_colors[i] * attenuation; 

        //cook-torrance BRDF
        float cosTheta = dot(H, V); //v - kierunek patrzenia
		vec3 F = fresnelSchlick(max(cosTheta,0.0), F0);

		float NDF = DistributionGGX(N, H, roughness);
		float G   = GeometrySmith(N, V, L, roughness);


		vec3 numerator    = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular     = numerator / max(denominator, 0.001);

		vec3 kS = F;
		vec3 kD = 1.0 - kS;
		kD *= 1.0 - metallic;
            
        //sum radiations
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    } 

    //vec3 ambient = vec3(0.03) * albedo *  ssao;
    vec3 ambient = albedo *  ssao;
    vec3 color   = ambient;// + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    color_texture = color;
    //FragColor = vec4(color, 1.0);
}