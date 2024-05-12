#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Light
{
	float intensity;
	/*glm::vec3 ambient_colour;
	glm::vec3 diffuse_colour;
	glm::vec3 specular_colour;*/
    glm::vec3 color;
    glm::vec3 position;
};

struct DirectionalLight : public Light
{
    glm::vec3 direction;
};

struct PointLight : public Light
{
};

struct SpotLight : public Light
{
    float cut_off;
    float outerCutOff;
    glm::vec3 direction;

    float constant;
    float linear;
    float quadratic;
};
#endif // LIGHTS_H
