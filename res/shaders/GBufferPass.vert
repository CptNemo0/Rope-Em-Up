#version 410 core
layout (location = 0) in vec3 iv_position;
layout (location = 1) in vec3 iv_normal;
layout (location = 2) in vec2 iv_texture;
layout (location = 5) in ivec4 iv_boneIds;
layout (location = 6) in vec4 iv_weights;

out vec3 world_position;
out vec3 view_position;
out vec2 uv;
out mat3 normal_world_matrix;
out mat3 normal_view_matrix;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	vec4 internal_world_position = model_matrix * vec4(iv_position, 1.0);
	vec4 internal_view_position = view_matrix * internal_world_position;

	world_position = internal_world_position.xyz;
	view_position = internal_view_position.xyz;

	uv = iv_texture;

	normal_world_matrix = transpose(inverse(mat3(model_matrix)));
	normal_view_matrix = transpose(inverse(mat3(view_matrix * model_matrix)));
	
	gl_Position = projection_matrix * internal_view_position;
}