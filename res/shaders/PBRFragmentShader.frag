#version 330 core

out vec4 FragColor;
in vec3 World_position;
in vec3 Normal;
in vec2 Texture_coords;

uniform vec3 light_positions[4];
uniform vec3 light_colors[4];

uniform vec3 camera_position;

uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D roughness_map;
uniform sampler2D ao_map;


// IBL
uniform samplerCube irradiance_map;
 
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
        vec3 N = normalize(Normal); 
        vec3 V = normalize(camera_position - World_position);
        vec3 R = reflect(-V, N); 


		vec3 albedo = texture(albedo_map, Texture_coords).rgb;
		float metallic = texture(metallic_map, Texture_coords).r;
		float roughness = texture(roughness_map, Texture_coords).r;
		float ao = texture(ao_map, Texture_coords).r;

        vec3 F0 = vec3(0.04);
        F0 = mix(F0, albedo, metallic);

        //The reflectance equation
		vec3 Lo = vec3(0.0);
        for(int i = 0; i < 4; ++i) 
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
        vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
        vec3 kD = 1.0 - kS;
        kD *= 1.0 - metallic;
        vec3 irradiance = texture(irradiance_map, N).rgb;
        vec3 diffuse      = irradiance * albedo;
        vec3 ambient = (kD * diffuse) * ao;

        vec3 color = ambient + Lo;

        // HDR tonemapping
        color = color / (color + vec3(1.0));
        // gamma correction
        color = pow(color, vec3(1.0/2.2)); 

        FragColor = vec4(color, 1.0);
}


