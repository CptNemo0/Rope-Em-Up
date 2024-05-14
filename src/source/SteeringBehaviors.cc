#include "../headers/SteeringBehaviors.h"

glm::vec3 Seek(glm::vec3 target, glm::vec3 actor)
{
	auto return_value = target - actor;
	return_value.y = 0;
	return glm::normalize(return_value);
}

glm::vec3 Flee(glm::vec3 target, glm::vec3 actor)
{
	auto return_value = actor - target;
	return_value.y = 0;
	return glm::normalize(return_value);
}

glm::vec3 Wander(s_ptr<components::Transform> actor, Vehicle& vehicle, float t)
{
	float x = random::RandFloat(-1, 1);
	float z = random::RandFloat(-1, 1);

	x *= vehicle.wander_jitter;
	z *= vehicle.wander_jitter;

	glm::vec3 target = vehicle.wander_target + glm::vec3(x, 0.0f, z);
	target = glm::normalize(target);
	target *= vehicle.wander_radius;
	target += actor->get_forward() * vehicle.wander_distance * t;

	vehicle.wander_target = glm::normalize(target);

	auto vel = (actor->get_position() - actor->get_previous_position()) / t;
	
	return glm::normalize(vel + target) * vehicle.wander_weight;
}

glm::vec3 WallAvoidance(s_ptr<components::Transform> actor, Vehicle& vehicle, float t)
{
	float up    = pbd::PBDManager::i_->walls_.up_left_.z;
	float left  = pbd::PBDManager::i_->walls_.up_left_.x;
	float down  = pbd::PBDManager::i_->walls_.down_right_.z;
	float right	= pbd::PBDManager::i_->walls_.down_right_.x;

	glm::vec3 raycast_f = actor->get_position() + actor->get_forward() * vehicle.wall_avoidance_distance;

	glm::vec3 return_value = glm::vec3(0.0f);

	if (up < raycast_f.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (down > raycast_f.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, +1.0f);
	}

	if (right > raycast_f.x)
	{
		return_value += glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (left < raycast_f.x)
	{
		return_value += glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	glm::vec3 raycast_r = actor->get_position() + actor->get_right() * vehicle.wall_avoidance_distance;

	if (up < raycast_r.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (down > raycast_r.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, +1.0f);
	}

	if (right > raycast_r.x)
	{
		return_value += glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (left < raycast_r.x)
	{
		return_value += glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	glm::vec3 raycast_l = actor->get_position() + -actor->get_right() * vehicle.wall_avoidance_distance;

	if (up < raycast_l.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (down > raycast_l.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, +1.0f);
	}

	if (right > raycast_l.x)
	{
		return_value += glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (left < raycast_l.x)
	{
		return_value += glm::vec3(+1.0f, 0.0f, 0.0f);
	}


	if (return_value == glm::vec3(0.0f, 0.0f, 0.0f))
	{
		return return_value;
	}
	else
	{
		return glm::normalize(return_value) * vehicle.wall_avoidance_weight;
	}
}

glm::vec3 Pursuit(glm::vec3 target, glm::vec3 target_forward, s_ptr<components::Transform> pursuer, Vehicle& vehicle, float t)
{
	glm::vec3 to_evader = target - pursuer->get_position();

	float relative_heading = glm::dot(pursuer->get_forward(), target_forward);

	if (glm::dot(to_evader, pursuer->get_forward()) > 0 && (relative_heading < -0.95))
	{
		return Seek(target, pursuer->get_position()) * vehicle.pursuit_weight;
	}

	return Seek(target + target_forward + vehicle.pursuit_distance, pursuer->get_position()) * vehicle.pursuit_weight;
}

glm::vec3 ExtrapolatedPursuit(glm::vec3 target, glm::vec3 target_forward, s_ptr<components::Transform> pursuer, Vehicle& vehicle, float t)
{
	return Pursuit(target + target_forward * vehicle.extrapolation_distance, target_forward, pursuer, vehicle, t) * vehicle.extrapolation_weight;
}

glm::vec3 Evade(glm::vec3 pursuer, glm::vec3 pursuer_forward, s_ptr<components::Transform> pursued, Vehicle& vehicle, float t)
{
	glm::vec3 to_pursuer = pursuer - pursued->get_position();

	return Flee(pursuer + pursuer_forward + vehicle.evade_distance, pursued->get_position()) * vehicle.evade_weight;
}
