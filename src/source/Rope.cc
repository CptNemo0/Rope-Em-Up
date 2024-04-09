#include "../headers/Rope.h"

bool rope::CheckRestraints(std::shared_ptr<RopeSegment> a, std::shared_ptr<RopeSegment> b)
{
	float distance = glm::distance(a->transform_->get_position(), b->transform_->get_position());
	if (distance >= kMaxDistance)
	{
		float dif = distance - kMaxDistance;
		glm::vec3 dir_b = glm::normalize(a->transform_->get_position() - b->transform_->get_position());
		glm::vec3 dir_a = glm::normalize(b->transform_->get_position() - a->transform_->get_position());

		//a->transform_->set_position(a->transform_->get_position() + dir_a * dif * 0.5f);
		//b->transform_->set_position(b->transform_->get_position() + dir_b * dif * 0.5f);

		auto particle_a = a->transform_->game_object_->GetComponent<Components::Particle>();
		auto particle_b = b->transform_->game_object_->GetComponent<Components::Particle>();
		assert(particle_a != nullptr);
		assert(particle_b != nullptr);

		glm::vec3 force = particle_b->transform_->get_position();
		force -= particle_a->transform_->get_position();
		float magnitude = glm::length(force);
		magnitude = std::abs(magnitude - kMaxDistance);
		magnitude *= kSpringConstant;
		glm::normalize(force);
		force *= -magnitude;
		//physics::ClampElementwise(force, 1.0f, -1.0f);
		//physics::LogVec3(force);
		particle_b->AddForce(force);

		force = particle_a->transform_->get_position();
		force -= particle_b->transform_->get_position();
		magnitude = glm::length(force);
		magnitude = std::abs(magnitude - kMaxDistance);
		magnitude *= kSpringConstant;
		glm::normalize(force);
		force *= -magnitude;
		//physics::ClampElementwise(force, 1.0f, -1.0f);
		//physics::LogVec3(force);
		particle_a->AddForce(force);
		

		return true;
	}

	return false;;
}