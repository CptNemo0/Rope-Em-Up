#include "../headers/SteeringBehaviors.h"

glm::vec3 Seek(std::shared_ptr<components::Transform> target, std::shared_ptr<components::Transform> actor)
{
	return glm::normalize(target->get_position() - actor->get_position());
}

glm::vec3 Flee(std::shared_ptr<components::Transform> target, std::shared_ptr<components::Transform> actor)
{
	return glm::normalize(actor->get_position() - target->get_position());
}

glm::vec3 Wander(std::shared_ptr<components::Transform> actor, Vehicle& vehicle, float t)
{
	float x = -1  + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2)));
	float z = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2)));

	x *= t;
	z *= t;

	vehicle.wander_target = glm::vec3(x, 0.0f, z);
	vehicle.wander_target = glm::normalize(vehicle.wander_target);
	vehicle.wander_target *= vehicle.wander_radius;

	glm::vec3 forward = actor->get_position() - actor->get_previous_position();
	if (forward.x != 0 && forward.y != 0 && forward.z != 0)
	{
		forward = glm::normalize(forward);
		forward *= vehicle.wander_distance;
	}
	else
	{
		forward = glm::vec3(0.0f, 0.0f, vehicle.wander_distance);
	}

	glm::vec3 target = vehicle.wander_target + forward; 

	std::cout << "target x: " << target.x << " y: " << target.y << " z;" << target.z << std::endl;

	glm::vec4 target4 = glm::vec4(target, 1.0f);
	target4 = actor->get_model_matrix() * target4;

	target.x = target4.x;
	target.y = target4.y;
	target.z = target4.z;

	std::cout << "t target x: " << target.x << " y: " << target.y << " z: " << target.z << std::endl;

	return glm::normalize(target - actor->get_position());
}
