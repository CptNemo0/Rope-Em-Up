#version 330 core
layout (location = 0) in vec3 iv_position;
layout (location = 1) in vec3 iv_normal;
layout (location = 2) in vec2 iv_texture;

out vec3 World_position;
out vec3 Normal;
out vec2 Texture_coords;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	vec4 mm = model_matrix * vec4(iv_position, 1.0);
	gl_Position = projection_matrix * view_matrix * mm;
	World_position = vec3(mm);
	Normal = normalize(vec3(transpose(inverse(model_matrix)) * vec4(iv_normal, 1.0)));;
	Texture_coords = iv_texture;
}