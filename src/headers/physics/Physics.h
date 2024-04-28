#ifndef PHYSICS_H
#define PHYSICS_H

#include "glm/glm.hpp"
#include <memory>
#include "../components/Transform.h"
#include <iostream>
#include "../components/Component.h"

struct FGRRecord;

class Particle;
class ForceGenerator;
class BasicGenerator;
class DragGenerator;
class PhysicsManager;

namespace components
{
	class Particle : public Component
	{
	public:
		float inverse_mass_;
		float mass_;
		
		float drag_;

		glm::vec3 forces_ = glm::vec3(0.0f);

		s_ptr<components::Transform> transform_;
		glm::vec3 velocity_ = glm::vec3(0.0f);
		glm::vec3 acceleration_ = glm::vec3(0.0f);

		Particle(s_ptr<components::Transform> transform, float mass, float drag) : transform_(transform), inverse_mass_(1.0f / mass), mass_(mass), drag_(drag) {}

		void UpdateAcceleration();
		void UpdateVelocity(float t);
		void UpdatePosition(float t);
		void AddForce(const glm::vec3& force);
		void ZeroForces();
		void UpdatePhysics(float t);

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override {}
	};
}

namespace physics
{
	const float kMaxForce = 100.0f;
	const float kCoeffiecentOfResitution = 0.5f;
	class ForceGenerator
	{
	public:
		ForceGenerator() = default;
		~ForceGenerator() = default;
		virtual void GenerateForce(s_ptr<components::Particle> particle) = 0;
	};

	class BasicGenerator : public ForceGenerator
	{
	public:
		float magnitude_;
		glm::vec3 direction_;

		BasicGenerator();
		~BasicGenerator() = default;

		// Inherited via ForceGenerator
		void GenerateForce(s_ptr<components::Particle> particle) override;

	};

	class DragGenerator : public ForceGenerator
	{
	public:
		float k1_;
		float k2_;

		DragGenerator(float k1, float k2);
		~DragGenerator() = default;

		void GenerateForce(s_ptr<components::Particle> particle) override;
	};

	//Force Generators Registry Record
	struct FGRRecord
	{
		s_ptr<components::Particle> particle;
		s_ptr<ForceGenerator> generator;
		
		void Generate();
	};

	struct Contact
	{
		Contact(s_ptr<components::Particle> p1, s_ptr<components::Particle> p2);
		s_ptr<components::Particle> a;
		s_ptr<components::Particle> b;
		glm::vec3 contact_normal;
	};

	class PhysicsManager
	{
	public:
		static PhysicsManager* i_;
	private:
		std::vector<FGRRecord> generator_registry_;
		std::vector<s_ptr<components::Particle>> particles_;
		PhysicsManager();
		~PhysicsManager() = default;
	public:
		static void Initialize()
		{
			if (i_ == nullptr)
			{
				i_ = new PhysicsManager();
			}
		}

		static void Destroy()
		{
			if (i_ != nullptr)
			{
				delete i_;
				i_ = nullptr;
			}
		}

		s_ptr<components::Particle> CreateParticle(s_ptr<components::Transform> transform, float mass, float drag);
		void GeneratorUpdate();
		void ParticleUpdate(float t);
		void AddFGRRecord(s_ptr<physics::ForceGenerator> generator, s_ptr<components::Particle> particle);
		void ResolveContact(physics::Contact& contact);
		void ResolveContacts(std::vector<physics::Contact> contacts);
		void RealizePositions();
	};

	void LogVec3(glm::vec3 v);
	float Clampf(float v, float max, float min);
	void ClampElementwise(glm::vec3& v, float max, float min);
	void Sigmoid(glm::vec3& v);
} //physics

#endif // !PHYSICS_H
