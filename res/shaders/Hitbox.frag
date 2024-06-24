#version 430 core

/*layout (location = 0) out vec3 position_texture;
layout (location = 1) out vec3 albedo_texture;
layout (location = 2) out vec3 normal_texture;
layout (location = 3) out vec3 mra_texture;
layout (location = 4) out vec3 view_position_texture;
layout (location = 5) out vec3 view_normal_texture;
layout (location = 6) out vec3 mask_texture;
layout (location = 7) out vec3 emissive_texture;*/

in vec3 world_position;
in vec2 uv;

uniform sampler2D albedo;
uniform vec4 color_in;

out vec4 FragColor;

void main()
{
	//position_texture = world_position;

	FragColor = texture(albedo, uv) * color_in; 

	/*albedo_texture = color ;
	normal_texture = vec3(0.0, 1.0, 0.0);
	mra_texture = vec3(0.5, 0.5, 0.0);
	view_position_texture = vec3(0.0, 0.0, 0.0);
	view_normal_texture = vec3(0.0, 0.0, 0.0);
	mask_texture = vec3(1.0);
	emissive_texture = color;*/

}