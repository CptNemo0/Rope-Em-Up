#ifndef PHYSICS_H
#define PHYSICS_H

#include "glm/glm.hpp"
#include <memory>
#include "Transform.h"

namespace physics
{
	class Particle;
	class ForceGenerator;

	class Particle
	{
	public:
		float inverse_mass_;
		glm::vec3 forces_ = glm::vec3(0.0f);

		std::shared_ptr<Components::Transform> transform_;
		glm::vec3 velocity_ = glm::vec3(0.0f);
		glm::vec3 acceleration_ = glm::vec3(0.0f);
		
		Particle(std::shared_ptr<Components::Transform> transform, float mass) : transform_(transform), inverse_mass_(1.0f / mass) {}
	
		void UpdateAcceleration();
		void UpdateVelocity(float t);
		void UpdatePosition(float t);
		void AddForce(glm::vec3 force);
		void ZeroForces();
		void UpdatePhysics(float t);
	};
	
	class ForceGenerator
	{
	public:
		ForceGenerator() = default;
		~ForceGenerator() = default;
		virtual void GenerateForce(std::shared_ptr<Particle>) = 0;
	};

	class DragGenerator : public ForceGenerator
	{
	public:
		float k1_;
		float k2_;

		DragGenerator(float k1, float k2);
		~DragGenerator() = default;

		void GenerateForce(std::shared_ptr<Particle>) override;
	};



} //physics

#endif // !PHYSICS_H
