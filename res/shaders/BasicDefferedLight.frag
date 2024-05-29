#version 330 core
layout (location = 0) out vec3 color_texture;

uniform sampler2D position_texture;
uniform sampler2D albedo_texture;
uniform sampler2D normal_texture;
uniform sampler2D mra_texture;
uniform sampler2D ssao_texture;

uniform vec3 camera_position;

const int MAX_LIGHTS = 16;
int light_num = 1;
uniform vec3 light_positions[MAX_LIGHTS];
uniform vec3 light_colors[MAX_LIGHTS];

in vec2 if_uv;

uniform samplerCube irradanceMap;

const float PI = 3.14159265359;



void main()
{
    vec3 position = texture(position_texture, if_uv).rgb;
    vec3 albedo =  texture(albedo_texture, if_uv).rgb;
    vec3 mra = texture(mra_texture, if_uv).rgb;
	float metallic = mra.r;
	float roughness = mra.g;
	float ao = mra.b;
    float ssao = texture(ssao_texture, if_uv).r;

    vec3 N = normalize(texture(normal_texture, if_uv).rgb * 2.0 - 1.0);
    vec3 V = normalize(camera_position - position);  

    vec3 ambient = vec3(1.0 * albedo * ssao);
    vec3 lighting = vec3(0.0, 0.0, 0.0);

    
    for(int i = 0 ; i < light_num && i < MAX_LIGHTS ; i++)
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


    ////////////////////////

    /*float dx = (1.0 / 1920.0) * 2;
    float dy = (1.0 / 1080.0) * 2;

    vec2 uvCenter   = if_uv;
    vec2 uvRight    = vec2(uvCenter.x + dx, uvCenter.y);
    vec2 uvTop      = vec2(uvCenter.x,      uvCenter.y - dx);
    vec2 uvTopRight = vec2(uvCenter.x + dx, uvCenter.y - dx);

    vec3 mCenter   = texture(normal_texture, uvCenter).rgb;
    vec3 mTop      = texture(normal_texture, uvTop).rgb;
    vec3 mRight    = texture(normal_texture, uvRight).rgb;
    vec3 mTopRight = texture(normal_texture, uvTopRight).rgb;

    vec3 dT  = abs(mCenter - mTop);
    vec3 dR  = abs(mCenter - mRight);
    vec3 dTR = abs(mCenter - mTopRight);

    float dTmax  = max(dT.x, max(dT.y, dT.z));
    float dRmax  = max(dR.x, max(dR.y, dR.z));
    float dTRmax = max(dTR.x, max(dTR.y, dTR.z));
   
    float deltaRaw = 0.0;
    deltaRaw = max(deltaRaw, dTmax);
    deltaRaw = max(deltaRaw, dRmax);
    deltaRaw = max(deltaRaw, dTRmax);

    float threshold    = 0.8;
    float deltaClipped = clamp((deltaRaw * 2.0) - threshold, 0.0, 1.0);
    float oI = deltaClipped;
    vec3 outline = vec3(oI, oI, oI);*/
    ////////////////////////


    vec3 color = ambient + lighting / light_num;   
    color = pow(color, vec3(1.0/2.2));
    color_texture = color;
}