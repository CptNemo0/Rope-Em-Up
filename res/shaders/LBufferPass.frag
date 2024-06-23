#version 330 core
layout (location = 0) out vec3 color_texture;
//layout (location = 1) out vec4 bloom_texture;

uniform sampler2D position_texture; //0
uniform sampler2D albedo_texture; //1
uniform sampler2D normal_texture;//2
uniform sampler2D mra_texture;//3
uniform sampler2D ssao_texture;//4

uniform samplerCube irradanceMap;//5
uniform samplerCube prefilterMap;//6
uniform sampler2D brdfLUT;//7

uniform sampler2D mask_texture; //8
uniform sampler2D emissive_texture; //9


uniform vec3 camera_position;

const int MAX_LIGHTS = 16;
uniform int light_num = 3;
uniform vec3 light_positions[MAX_LIGHTS];
uniform vec3 light_colors[MAX_LIGHTS];
uniform  vec4 frag_pos_light_space;
uniform float far_plane;

uniform samplerCube cube_shadow_maps[MAX_LIGHTS]; //10
uniform sampler2D plane_shadow_maps[MAX_LIGHTS]; //10 + 16 = 26

uniform bool slowed_time;

uniform bool use_ssao;

in vec2 if_uv;


const float PI = 3.14159265359;

struct DirLight {
    float intensity;
    vec3 position;
    vec3 direction;

    vec3 color;
    mat4 light_space_matrix;
    int ID;
};

struct PointLight {
    float intensity;
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 color;
    mat4 light_space_matrix;
    int ID;
};

struct SpotLight {
	float intensity;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 color;
};

uniform PointLight pointLight[MAX_LIGHTS];
uniform DirLight dirLight[1];
uniform SpotLight spotLight[1];

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

float DistributionGGX(vec3 N, vec3 H, float roughness) //funkcja rozk�adu wektor�w normalnych Trowbridge-Reitz GGX
{
		float a      = roughness * roughness;
        float a2     = a*a;
        float NdotH  = max(dot(N, H), 0.01);
        float NdotH2 = NdotH*NdotH;

        float nom    = a2;
        float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
        denom        = max(PI * denom * denom, 0.00001);

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

vec3 CalcDirLight(DirLight light, vec3 V, vec3 N, float roughness, float metallic, vec3 albedo, vec3 F0)
{
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);
    vec3 radiance = light.color * light.intensity;

// Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0); 
    
    vec3 numerator    = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular = numerator / max(denominator, 0.00001);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;
        
    //sum radiations
	float NdotL = max(dot(N, L), 0.0);
	return (kD * albedo / PI + specular) * radiance * NdotL;  
}

vec3 CalcPointLight(PointLight light, vec3 World_position, vec3 V, vec3 N, float roughness, float metallic, vec3 albedo, vec3 F0)
{
    vec3 L = normalize(light.position - World_position);
	vec3 H = normalize(V + L);
	float distance = length(light.position - World_position);
	float attenuation = 1.0 / max(light.constant + light.linear * distance + light.quadratic * (distance * distance), 0.00001);
	vec3  radiance = light.color * attenuation * light.intensity;

// Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0); 
    
    vec3 numerator    = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular = numerator / max(denominator, 0.00001);

    vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= vec3(1.0) - metallic;
        
    //sum radiations
	float NdotL = max(dot(N, L), 0.0);
	return (kD * albedo / PI + specular) * radiance * NdotL; 
}


vec3 CalcSpotLight(SpotLight light, vec3 World_position, vec3 V, vec3 N, float roughness, float metallic, vec3 albedo, vec3 F0)
{
	vec3 L = normalize (light.position - World_position);
	vec3 H = normalize(V + L);
	float distance = length(light.position - World_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(L, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 radiance = light.color * attenuation  * light.intensity * intensity;
// Cook-Torrance BRDF
	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0); 
	
	vec3 numerator = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular = numerator / max(denominator, 0.00001);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= vec3(1.0) - metallic;
		
	//sum radiations
	float NdotL = max(dot(N, L), 0.0);
	return (kD * albedo / PI + specular) * radiance * NdotL;
}

float PlaneShadowCalculation(mat4x4 lightSpaceMatrix, vec3 lightPos, int lighyId, vec3 N, vec3 WordPosition)
{
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(lightPos, 1.0);

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(plane_shadow_maps[lighyId], projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    //vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - WordPosition);
    float bias = max(0.05 * (1.0 - dot(N, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(plane_shadow_maps[lighyId], 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(plane_shadow_maps[lighyId], projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

float PointShadowCalculation(vec3 fragPos, vec3 lightPos, samplerCube shadowCubeMap, vec3 N, vec3 WordPosition)
{
    // Calculate the vector from the fragment position to the light position
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    
    // Transform the vector to the [0,1] range
    vec3 projCoords = fragToLight / currentDepth;
    
    // Get the closest depth value from the light's perspective
    //float closestDepth = texture(shadowCubeMap, projCoords).r * far_plane;
    
    // Calculate bias to prevent shadow acne
    float bias = 0.05;
    
    // Perform percentage-closer filtering (PCF)
    float shadow = 0.0;
    int samples = 20;
    float offset = 1.0;
    for(int x = -samples; x < samples; ++x)
    {
        for(int y = -samples; y < samples; ++y)
        {
            for(int z = -samples; z < samples; ++z)
            {
                float pcfDepth = texture(shadowCubeMap, projCoords + vec3(x, y, z) * offset / far_plane).r * far_plane;
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
    }
    shadow /= (samples * samples * samples);
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum
    if (currentDepth > far_plane)
        shadow = 0.0;
    
    return shadow;
}



void main()
{
    float mask =  texture(mask_texture, if_uv).r;
    vec3 albedo = texture(albedo_texture, if_uv).rgb;
    vec3 emission = texture(emissive_texture, if_uv).rgb;

    if(mask > 0)
    {
        vec3 World_position = texture(position_texture, if_uv).rgb;   
        vec3 mra = texture(mra_texture, if_uv).rgb;
	    float metallic = mra.r;
	    float roughness = mra.g;
	    float ao = mra.b;
        float ssao = ((texture(ssao_texture, if_uv).r - 0.5) * 1.5) + 0.5;
        

        vec3 N = normalize(texture(normal_texture, if_uv).rgb * 2.0 - 1.0);
        vec3 V = normalize(camera_position - World_position);
        vec3 R = reflect(-V, N);

        vec3 F0 = vec3(0.04);
        F0 = mix(F0, albedo, metallic);

        //The reflectance equation
	    vec3 Lo = vec3(0.0);
        for(int i = 0; i < light_num && i <MAX_LIGHTS; ++i) 
        {
		    Lo += CalcPointLight(pointLight[i], World_position, V, N, roughness, metallic, albedo, F0);
        }

        Lo += CalcDirLight(dirLight[0], V, N, roughness, metallic, albedo, F0);
        Lo += CalcSpotLight(spotLight[0], World_position, V, N, roughness, metallic, albedo, F0);

        /// ambient lighting IBL ///
        vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
        vec3 kD = 1.0 - kS;
        kD *= 1.0 - metallic;	
  
        vec3 irradiance = texture(irradanceMap, N).rgb;
        vec3 diffuse = irradiance * albedo;

        const float MAX_REFLECTION_LOD = 4.0;
        vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
        vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
        vec3 specular = prefilteredColor * (kS * brdf.x + brdf.y);

        vec3 ambient = (kD * diffuse) * ao;
        
        vec3 color = vec3(0.0);
        if (emission != vec3(0.0))
        {
            float hm = dot(albedo, emission);
            color = albedo * (1 - hm) + emission * hm;
        }
        else
        {
            color = ambient + Lo;
        }

        if(use_ssao)
        {
            color = color * ssao;
        }

        if(slowed_time)
        {
            color = vec3( dot(color, vec3(0.2126, 0.7152, 0.0722)) );
        }

        color_texture = color;   
    }
    else
    {
        if(slowed_time)
        {
            albedo = vec3( dot(albedo, vec3(0.2126, 0.7152, 0.0722)) );
        }
        color_texture = albedo + emission;
    }
}