#ifndef VEHICLE_H
#define VEHICLE_H

#include "glm/glm.hpp"

struct Vehicle
{
	float rest_lenght;

	glm::vec3 wander_target;
	float wander_radius;
	float wander_distance;
	float wander_weight;
	float wander_jitter;

	float max_speed;

	float wall_avoidance_distance;
	float wall_avoidance_weight;

	float pursuit_range;
	float pursuit_distance;
	float pursuit_weight;

	float extrapolation_distance;
	float extrapolation_weight;
	
	float evade_distance;
	float evade_range;
	float evade_weight;
};

#endif