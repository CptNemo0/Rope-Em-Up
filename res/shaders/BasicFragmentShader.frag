#version 330 core

struct PointLight
{
    float intensity;
    vec3 position;
	vec3 ambient_colour;
	vec3 diffuse_colour;
	vec3 specular_colour;
};

in vec3 if_normal;
in vec2 if_texture;
in vec3 if_fragment_position;

uniform sampler2D this_texture;
uniform PointLight light;
uniform float shininess;

float GetDistanceFactor(float distance, float intensity)
{
    return (intensity / (distance + 1));
}

vec3 PointLightColour(PointLight light, vec3 view_direction)
{
    vec3 light_direction = light.position - if_fragment_position;
    vec3 light_direction_normalized = normalize(light_direction);

    float distance = length(light_direction);
    distance = distance * distance;
    float distance_factor = GetDistanceFactor(distance, light.intensity);

    float diffuse = max(dot(light_direction_normalized, if_normal), 0.0);

    float specular = 0.0f;
    if(diffuse != 0)
    {
        
        vec3 half_direction = normalize(light_direction + view_direction);
        float cosine = max(dot(half_direction, if_normal), 0.0);
        specular = pow(cosine, shininess);
    }
    
    return light.ambient_colour + light.diffuse_colour * diffuse * distance_factor + light.specular_colour * specular * distance_factor;
}

out vec4 FragColour;

void main()
{
   vec3 view_direction = normalize(if_fragment_position - vec3(0.0f, 5.0f, -5.0f));
   vec3 result = PointLightColour(light, view_direction);
   FragColour = texture(this_texture, if_texture) * vec4(result, 1.0f); 
}