#version 330 core
layout (location = 0) out vec3 downscaled_texture;
layout (location = 1) out vec3 loc1;
layout (location = 2) out vec3 loc2;

in vec2 if_uv;

uniform sampler2D color_texture; 
uniform sampler2D emission_texture; 

void main()
{
	vec3 color = texture(color_texture, if_uv).rgb;
	vec3 emission = texture(emission_texture, if_uv).rgb;

	float l = length(color);

	if(emission != vec3(0.0))
	{
		downscaled_texture = emission;
	}
	else
	{
		downscaled_texture = (- (l - 0.5) * (l * 1.5) * 4.0) * color;
	}

	loc1 = vec3(0.0);
	loc2 = vec3(1.0);
}
