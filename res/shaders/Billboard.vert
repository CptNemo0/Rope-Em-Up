#version 430 core
layout (location = 0) in vec3 iv_position;
layout (location = 2) in vec2 iv_texture;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

/*out VS_OUT
{
	vec2 uv;
} vs_out;*/

out vec2 uv;

void main()
{
	uv = iv_texture;
	
	mat4 mm = model_matrix;

	mm[0][0] = 1;
	mm[0][1] = 0;
	mm[0][2] = 0;

	// Column 1:
	mm[1][0] = 0;
	mm[1][1] = 1;
	mm[1][2] = 0;

	// Column 2:
	mm[2][0] = 0; 
	mm[2][1] = 0;
	mm[2][2] = 1;
	mat4 model_view =  view_matrix * mm;
	gl_Position = projection_matrix * model_view * vec4(iv_position, 1.0);
	//vec3 camera_right = vec3(view_matrix[0][0], view_matrix[1][0], view_matrix[2][0]);
    //vec3 camera_up = vec3(view_matrix[0][1], view_matrix[1][1], view_matrix[2][1]);

	//vec3 billboard_position = model_matrix[3].xyz + camera_right * iv_position.x + camera_up * iv_position.y;
	//gl_Position = projection_matrix * view_matrix * vec4(billboard_position, 1.0);
	//gl_Position = projection_matrix * model_view * vec4(iv_position, 1.0);
}