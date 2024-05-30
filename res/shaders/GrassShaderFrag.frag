#version 430

layout (location = 0) out vec3 position_texture;
layout (location = 1) out vec3 albedo_texture;
layout (location = 2) out vec3 normal_texture;
layout (location = 3) out vec3 mra_texture;
layout (location = 4) out vec3 view_position_texture;
layout (location = 5) out vec3 view_normal_texture;
layout (location = 6) out vec3 mask_texture;

in vec3 out_world_position;
in vec3 out_normal;
in vec3 out_view_position;
in vec2 out_uv;
in mat3 out_normal_world_matrix;
in mat3 out_normal_view_matrix;

void main()
{
	position_texture = out_world_position;
	
	vec3 start_color = vec3(0.0705, 0.4196, 0.074509);
	vec3 end_color = vec3(0.47843, 0.3882, 0.19607);
	float sample_point = out_uv.x * out_uv.x * 10;
	albedo_texture = mix(start_color, end_color, sample_point);

	normal_texture = normalize(vec3(out_uv.x * 2.0 - 1.0, 0.0, out_uv.y * 2.0 - 1.0))* 0.5 + 0.5;
	mra_texture = vec3(0.3, 0.3, 0.0);
	view_position_texture = out_view_position;
	view_normal_texture= (normalize(out_normal_view_matrix * out_normal) + 1.0) * 0.5;
	mask_texture = vec3(1.0);
}