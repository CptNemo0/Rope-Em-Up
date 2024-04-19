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

glm::vec3 Wander(std::shared_ptr<components::Transform> actor, Vehicle& vehicle, float t)
{
	float x = -1  + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2)));
	float z = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2)));

	x *= vehicle.wander_jitter;
	z *= vehicle.wander_jitter;

	glm::vec3 target = vehicle.wander_target + glm::vec3(x, 0.0f, z);
	target = glm::normalize(target);
	target *= vehicle.wander_radius;
	target += actor->get_forward() * vehicle.wander_distance * t;

	vehicle.wander_target = glm::normalize(target);

	auto vel = (actor->get_position() - actor->get_previous_position()) / t;
	
	return glm::normalize(vel + target);
}

glm::vec3 WallAvoidance(std::shared_ptr<components::Transform> actor, Vehicle& vehicle, float t)
{
	float up    = pbd::PBDManager::i_->walls_.up_left_.z;
	float left  = pbd::PBDManager::i_->walls_.up_left_.x;
	float down  = pbd::PBDManager::i_->walls_.down_right_.z;
	float right	= pbd::PBDManager::i_->walls_.down_right_.x;

	glm::vec3 raycast = actor->get_position() + actor->get_forward() * vehicle.wall_avoidance_distance;

	glm::vec3 return_value = glm::vec3(0.0f);

	if (up < raycast.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (down > raycast.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, +1.0f);
	}

	if (right < raycast.x)
	{
		return_value += glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	else if (left > raycast.x)
	{
		return_value += glm::vec3(1.0f, 0.0f, 0.0f);
	}

	raycast = actor->get_position() + actor->get_right() * vehicle.wall_avoidance_distance;

	if (up < raycast.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (down > raycast.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, +1.0f);
	}

	if (right < raycast.x)
	{
		return_value += glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	else if (left > raycast.x)
	{
		return_value += glm::vec3(1.0f, 0.0f, 0.0f);
	}

	raycast = actor->get_position() + -actor->get_right() * vehicle.wall_avoidance_distance;

	if (up < raycast.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (down > raycast.z)
	{
		return_value += glm::vec3(0.0f, 0.0f, +1.0f);
	}

	if (right < raycast.x)
	{
		return_value += glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	else if (left > raycast.x)
	{
		return_value += glm::vec3(1.0f, 0.0f, 0.0f);
	}


	if (return_value == glm::vec3(0.0f, 0.0f, 0.0f))
	{
		return return_value;
	}
	else
	{
		return glm::normalize(return_value);
	}
}

glm::vec3 Pursuit(std::shared_ptr<components::Transform> target, std::shared_ptr<components::Transform> pursuer, Vehicle& vehicle, float t)
{
	/*if (glm::distance(target->get_position(), pursuer->get_position()) < 2.0f)
	{
		return glm::vec3(0.0f);
	}*/

	glm::vec3 to_evader = target->get_position() - pursuer->get_position();

	float relative_heading = glm::dot(pursuer->get_forward(), target->get_forward());

	if (glm::dot(to_evader, pursuer->get_forward()) > 0 && (relative_heading < -0.95))
	{
		return Seek(target->get_position(), pursuer->get_position());
	}

	return Seek(target->get_position() + target->get_forward() + vehicle.look_ahead_distance, pursuer->get_position());
}
