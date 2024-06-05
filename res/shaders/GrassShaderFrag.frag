#version 430
layout (location = 0) out vec3 position_texture;
layout (location = 1) out vec3 albedo_texture;
layout (location = 2) out vec3 normal_texture;
layout (location = 3) out vec3 mra_texture;
layout (location = 4) out vec3 view_position_texture;
layout (location = 5) out vec3 view_normal_texture;
layout (location = 6) out vec3 mask_texture;

in vec3 out_world_position;
in vec3 out_view_position;
in vec2 out_uv;
in mat3 out_normal_world_matrix;
in mat3 out_normal_view_matrix;
in vec3 T;
in vec3 B;
in vec3 N;

uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D roughness_map;
uniform sampler2D ao_map;

void main()
{
	mat3 TBN = mat3(T, B, N);
	float metallic = texture(metallic_map, out_uv).r;
	float roughness = texture(roughness_map, out_uv).r;
	float ao = texture(ao_map, out_uv).r;
	/*vec3 normal = texture(normal_map, out_uv).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(TBN * normal);*/
	
	vec3 start_color = vec3(0.08627, 0.26666, 0.0549);
	vec3 end_color = vec3(0.81176, 0.92549, 0.588235);
	float sample_point = 1 - out_uv.y - 0.616;
	
	position_texture = out_world_position;
	albedo_texture = mix(start_color, end_color, sample_point);
	normal_texture = (normalize(out_normal_world_matrix * N) + 1.0) * 0.5;
	mra_texture = vec3(0.0, 1.0, 0.0);

	view_position_texture = out_view_position;
	view_normal_texture= (normalize(out_normal_view_matrix * N) + 1.0) * 0.5;
	mask_texture = vec3(1.0);
}