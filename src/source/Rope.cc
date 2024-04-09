#include "../headers/Rope.h"

bool rope::CheckRestraints(std::shared_ptr<RopeSegment> a, std::shared_ptr<RopeSegment> b, float t)
{
	float distance = glm::distance(a->transform_->get_position(), b->transform_->get_position());
	if (distance >= kMaxDistance)
	{
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
		particle_b->AddForce(force);
		

		force = particle_a->transform_->get_position();
		force -= particle_b->transform_->get_position();
		magnitude = glm::length(force);
		magnitude = std::abs(magnitude - kMaxDistance);
		magnitude *= kSpringConstant;
		glm::normalize(force);
		force *= -magnitude;
		particle_a->AddForce(force);
		
		

		return true;
	}

	return false;;
}