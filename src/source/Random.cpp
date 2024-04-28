#include "../headers/Random.h"

float random::RandFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int random::RandInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}

glm::vec3 random::RandInsideSphere()
{
    float u = RandFloat(0.0f, 1.0f);
    float v = RandFloat(0.0f, 1.0f);
    float theta = u * 2.0f * glm::pi<float>();
    float phi = glm::acos(2.0f * v - 1.0f);
    float r = std::cbrtf(RandFloat(0.0f, 1.0f));
    float sin_theta = glm::sin(theta);
    float cos_theta = glm::cos(theta);
    float sin_phi = glm::sin(phi);
    float cos_phi = glm::cos(phi);
    return glm::vec3(r * sin_phi * cos_theta, r * sin_phi * sin_theta, r * cos_phi);
}
