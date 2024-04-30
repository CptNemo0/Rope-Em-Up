#version 410 core
layout (location = 0) in vec3 iv_position;
layout (location = 1) in vec3 iv_normal;
layout (location = 2) in vec2 iv_texture;

out vec3 world_pos;
out vec2 uv;
out mat4 normal_matrix;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	vec4 mm = model_matrix * vec4(iv_position, 1.0);
	world_pos = vec3(mm);
	uv = iv_texture;
	normal_matrix = transpose(inverse(model_matrix));
	gl_Position = projection_matrix * view_matrix * mm;	
}