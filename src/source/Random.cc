#include "../headers/Random.h"

std::random_device rd;
std::mt19937 g(rd());

std::mt19937 &random::get_random_device()
{
    return g;
}

float random::RandFloat(float min, float max)
{
    return min + (static_cast<float>(g()) / static_cast<float>(g.max())) * (max - min);
}

float random::RandFloatExcl(float min, float max)
{
    return min + (static_cast<float>(g()) / static_cast<float>(g.max() + 1)) * (max - min);
}

int random::RandInt(int min, int max)
{

    return min + g() % (max - min + 1);
}

int random::RandIntExcl(int min, int max)
{
    return min + g() % (max - min);
}

glm::vec3 random::RandInsideSphere()
{
    float u = RandFloat(0.0f, 1.0f);
    float v = RandFloat(0.0f, 1.0f);
    float theta = u * glm::two_pi<float>();
    float phi = glm::acos(2.0f * v - 1.0f);
    float r = std::cbrtf(RandFloat(0.0f, 1.0f));
    float sin_theta = glm::sin(theta);
    float cos_theta = glm::cos(theta);
    float sin_phi = glm::sin(phi);
    float cos_phi = glm::cos(phi);
    return glm::vec3(r * sin_phi * cos_theta, r * sin_phi * sin_theta, r * cos_phi);
}

glm::vec2 random::RandOnCircle()
{
    float theta = RandFloatExcl(0.0f, 1.0f) * glm::two_pi<float>();
    return glm::vec2(glm::cos(theta), glm::sin(theta));
}

glm::vec2 random::RandOnArc(float angle, float span)
{
    float span_quarter = span / 4.0f;
    float theta = (angle + RandFloat(-span_quarter, span_quarter)) * glm::two_pi<float>();
    return glm::vec2(glm::cos(theta), glm::sin(theta));
}
