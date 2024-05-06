#version 410 core
layout (location = 0) out vec3 position_texture;
layout (location = 1) out vec3 albedo_texture;
layout (location = 2) out vec3 normal_texture;
layout (location = 3) out vec3 mra_texture;
layout (location = 4) out vec3 view_position_texture;
layout (location = 5) out vec3 view_normal_texture;
layout (location = 6) out vec3 depth_texture;

in vec3 world_position;
in vec3 view_position;
in vec2 uv;
in mat3 normal_world_matrix;
in mat3 normal_view_matrix;

uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D roughness_map;
uniform sampler2D ao_map;

out vec4 color;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * 0.1 * 100) / (100 + 0.1 - z * (100 - 0.1));	
}

void main()
{
	float metallic = texture(metallic_map, uv).r;
	float roughness = texture(roughness_map, uv).r;
	float ao = texture(ao_map, uv).r;
	vec3 normal = texture(normal_map, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	position_texture = world_position;
	albedo_texture = texture(albedo_map, uv).rgb;
	normal_texture = normalize(normal_world_matrix * normal) + 1.0 * 0.5;
	mra_texture = vec3(metallic, roughness, ao);
	view_position_texture = view_position;
	view_normal_texture = normalize(normal_view_matrix * normal) + 1.0 * 0.5;
	
	float depth = LinearizeDepth(gl_FragCoord.z) * 0.01;
	depth_texture = vec3(gl_FragCoord.z);
}