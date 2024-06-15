#version 460 core
layout (location = 0) out vec3 position_texture;
layout (location = 1) out vec3 albedo_texture;
layout (location = 2) out vec3 normal_texture;
layout (location = 3) out vec3 mra_texture;
layout (location = 4) out vec3 view_position_texture;
layout (location = 5) out vec3 view_normal_texture;
layout (location = 6) out vec3 mask_texture;
layout (location = 7) out vec3 emissive_texture;

in vec3 world_position;
in vec2 uv;

uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D roughness_map;

void main()
{
	mat3 TBN = mat3(vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0));
	vec3 normal = texture(normal_map, uv).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(TBN * normal);

	position_texture = world_position;
	albedo_texture = texture(albedo_map, uv).rgb;
	normal_texture = (normal + 1.0) * 0.5;
	mra_texture = vec3(texture(roughness_map, uv).r, texture(roughness_map, uv).r, 0.0);
	
	view_position_texture = vec3(0.0);
	view_normal_texture = vec3(0.0);
	mask_texture = vec3(1.0);
	emissive_texture = vec3(0.0);
}