#version 330 core
layout (location = 0) out vec3 color_texture;

uniform sampler2D position_texture;
uniform sampler2D albedo_texture;
uniform sampler2D normal_texture;
uniform sampler2D mra_texture;
uniform sampler2D ssao_texture;
uniform sampler2D tangent_texture;
uniform sampler2D bitangent_texture;

uniform vec3 camera_position;

const int MAX_LIGHTS = 16;
uniform int light_num = 3;
uniform vec3 light_positions[MAX_LIGHTS];
uniform vec3 light_colors[MAX_LIGHTS];

in vec2 if_uv;

uniform samplerCube irradianceMap;

const float PI = 3.14159265359;

struct DirLight {
    float intensity;
    vec3 direction;

    vec3 color;
};

struct PointLight {
    float intensity;
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 color;
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

void main()
{
    vec3 position = texture(position_texture, if_uv).rgb;
    vec3 albedo = texture(albedo_texture, if_uv).rgb;
    vec3 mra = texture(mra_texture, if_uv).rgb;
	float metallic = mra.r;
	float roughness = mra.g;
	float ao = mra.b;
    float ssao = ((texture(ssao_texture, if_uv).r - 0.5) * 1.5) + 0.5;
    
    vec3 normal = normalize(texture(normal_texture, if_uv).rgb * 2.0 - 1.0);
    /*vec3 T = normalize(texture(tangent_texture, if_uv).rgb * 2.0 - 1.0);
    vec3 B = normalize(texture(bitangent_texture, if_uv).rgb * 2.0 - 1.0);

    mat3 TBN = mat3(T, B, normal);*/

    vec3 view_dir = normalize(camera_position - position);

    vec3 ambient = vec3(0.1);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    vec3 color = vec3(0.0);

    float num_inverse = 1.0 / light_num;

    vec3 light_dir;

    for(int i = 0; i < light_num && i < MAX_LIGHTS ; i++)
    {
        light_dir = normalize(pointLight[i].position - position);
        float diff = max(dot(normal, light_dir), 0.0);
        float r_diffuse = distance(position, pointLight[i].position);
        float fall_of_diffuse = 1.0 / ( r_diffuse * r_diffuse);
        diff = diff * pointLight[i].intensity * fall_of_diffuse;

        diffuse += pointLight[i].color * diff;

        vec3 reflectDir = reflect(-light_dir, normal);  
        float spec = pow(max(dot(view_dir, reflectDir), 0.0), 32);
        specular += 0.5 * spec * pointLight[i].color;  
    }

    diffuse *= num_inverse;
    specular *= num_inverse;

    color = (ambient + diffuse + specular) * albedo;
    color_texture = color * ssao;
    
}