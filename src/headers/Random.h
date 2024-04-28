#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

namespace random
{

// Inclusive min and max
float RandFloat(float min, float max);

// Inlcusive min and max
int RandInt(int min, int max);

// Returns a random point inside a unit sphere
glm::vec3 RandInsideSphere();

};

#endif // !RANDOM_H