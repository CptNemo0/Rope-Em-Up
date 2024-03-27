#version 330 core
layout (location = 0) in vec3 iv_position;
layout (location = 1) in vec3 iv_normal;
layout (location = 2) in vec2 iv_texture;

out vec3 if_fragment_position;
out vec3 if_normal;
out vec2 if_texture;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	vec4 mm = model_matrix * vec4(iv_position, 1.0);
	gl_Position = projection_matrix * view_matrix * mm;
	if_fragment_position = vec3(mm);
	if_normal = normalize(vec3(transpose(inverse(model_matrix)) * vec4(iv_normal, 1.0)));;
	if_texture = iv_texture;
}