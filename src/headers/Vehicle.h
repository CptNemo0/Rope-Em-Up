#ifndef VEHICLE_H
#define VEHICLE_H

#include "glm/glm.hpp"

struct Vehicle
{
	glm::vec3 wander_target;
	float wander_radius;
	float wander_distance;

	float max_speed;
};

#endif