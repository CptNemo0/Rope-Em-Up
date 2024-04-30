#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include "global.h"

namespace random
{

std::mt19937 &get_random_device();

// Inclusive min and max
float RandFloat(float min, float max);
// Inclusive min, exclusive max
float RandFloatExcl(float min, float max);

// Inlcusive min and max
int RandInt(int min, int max);
// Inclusive min, exclusive max
int RandIntExcl(int min, int max);

// Returns a random point inside a unit sphere
glm::vec3 RandInsideSphere();
// Returns a random point on the unit circle
glm::vec2 RandOnCircle();
/// Returns a random point on an arc of a circle.
/// Angle must be [0.0, 1.0]
/// Angle 0.0 is at (1.0, 0.0) and higher values go counter-clockwise
/// Span 1.0 is half a circle, 0.5 is a quarter circle, etc.
glm::vec2 RandOnArc(float angle, float span);

};

#endif // !RANDOM_H