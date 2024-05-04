#version 410 core
layout (location = 0) out vec3 position_texture;
layout (location = 1) out vec3 albedo_texture;
layout (location = 2) out vec3 normal_texture;
layout (location = 3) out vec3 mra_texture;

in vec3 world_pos;
in vec2 uv;
in mat3 normal_matrix;

uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D roughness_map;
uniform sampler2D ao_map;

//out vec4 FragColor;

void main()
{
	float metallic = texture(metallic_map, uv).r;
	float roughness = texture(roughness_map, uv).r;
	float ao = texture(ao_map, uv).r;
	vec3 normal = texture(normal_map, uv).rgb;
	
	position_texture = world_pos;
	albedo_texture = texture(albedo_map, uv).rgb;
	normal_texture = normal;
	mra_texture = vec3(metallic, roughness, ao);

	//FragColor = vec4(normal_texture, 1.0f);
}