#version 410 core
layout (location = 0) out vec3 position_texture;
layout (location = 1) out vec3 albedo_texture;
layout (location = 2) out vec3 normal_texture;
layout (location = 3) out vec3 mra_texture;

//out vec4 color;

in vec3 world_pos;
in vec2 uv;

uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D roughness_map;
uniform sampler2D ao_map;


void main()
{
	float metallic = texture(metallic_map, uv).r;
	float roughness = texture(roughness_map, uv).r;
	float ao = texture(ao_map, uv).r;

	position_texture = world_pos;
	albedo_texture = texture(albedo_map, uv).rgb;
	normal_texture = texture(normal_map, uv).rgb;
	mra_texture = vec3(metallic, roughness, ao);

	//color = vec4(vec3(roughness), 1.0f);
}