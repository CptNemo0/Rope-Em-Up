#ifndef PBD_H
#define PBD_H

#include <iostream>
#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "Component.h"
#include "Transform.h"

namespace Components
{
	class PBDParticle : public Component, public std::enable_shared_from_this<PBDParticle>
	{
	public:
		std::shared_ptr<Components::Transform> transform_;
		float mass_;
		float inverse_mass_;
		float damping_factor_;
		glm::vec3 velocity_;
		glm::vec3 forces_;

		PBDParticle(float mass, float damping_factor, std::shared_ptr<Components::Transform> transform) : 
		mass_(mass), 
		damping_factor_(damping_factor), 
		transform_(transform), 
		inverse_mass_(1.0f/mass), 
		velocity_(glm::vec3(0.0f)),
		forces_(glm::vec3(0.0f)) {}

		void AddForce(glm::vec3 force);
		void ZeroForces();
		void SympleticEulerIntegration(float t);
		void DampVelocity();
		void UpdateVelocity(float t);
		void PredictPosition(float t);
		void UpdatePosition(float t);

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;

		~PBDParticle();
	};
}

namespace pbd
{
	class ForceGenerator
	{
	public:
		ForceGenerator() = default;
		~ForceGenerator() = default;
		virtual void GenerateForce(std::shared_ptr<Components::PBDParticle> particle) = 0;
	};

	class BasicGenerator : public ForceGenerator
	{
	public:
		float magnitude_;
		glm::vec3 direction_;

		BasicGenerator() : 
		magnitude_(0.0f), direction_(glm::vec3(0.0f)) {}
		~BasicGenerator() = default;

		// Inherited via ForceGenerator
		void GenerateForce(std::shared_ptr<Components::PBDParticle> particle) override;
	};

	struct FGRRecord
	{
		std::shared_ptr<Components::PBDParticle> particle;
		std::shared_ptr<ForceGenerator> generator;

		FGRRecord(std::shared_ptr<Components::PBDParticle> p, std::shared_ptr<ForceGenerator> g) :
		particle(p), generator(g) {}
		FGRRecord() :
		particle(nullptr), generator(nullptr) {}
		

		void Generate();
	};

	class Constraint
	{
	public:
		float k_;
		Constraint() = default;
		~Constraint() = default;
		virtual void Enforce()
		{
			return;
		}
	};

	class RopeConstraint : public Constraint
	{
	public:

		RopeConstraint(std::shared_ptr<Components::PBDParticle> p1, std::shared_ptr<Components::PBDParticle> p2, float ml);
		RopeConstraint() = default;
		~RopeConstraint() = default;
		std::shared_ptr<Components::PBDParticle> p1_;
		std::shared_ptr<Components::PBDParticle> p2_;
		
		float max_distance_;

		void Enforce() override;
	};

	struct Contact
	{
		Contact(std::shared_ptr<Components::PBDParticle> p1, std::shared_ptr<Components::PBDParticle> p2);
		~Contact();
		std::shared_ptr<Components::PBDParticle> a;
		std::shared_ptr<Components::PBDParticle> b;
		glm::vec3 contact_normal;
	};

	class PBDManager
	{
    public:
        static PBDManager* i_;
    private:
		PBDManager(int it, float coeffiecent_of_restitution);
        ~PBDManager() = default;
    public:

		std::vector<std::shared_ptr<Components::PBDParticle>> particles_;
		std::vector<pbd::FGRRecord> generator_registry_;
		std::vector<pbd::RopeConstraint> constraints_;
		std::vector<pbd::Contact> contacts_;

		int solver_iterations_;
		float coeffiecent_of_restitution_;

        static void Initialize(int it, float coeffiecent_of_restitution)
        {
            if (i_ == nullptr)
            {
                i_ = new PBDManager(it, coeffiecent_of_restitution);
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

		void RemoveRecord(std::shared_ptr<ForceGenerator> g);
		void RemoveRecord(std::shared_ptr<Components::PBDParticle> p);

		void Integration(float t);
		
		void ProjectConstraints(float t);

		std::shared_ptr<Components::PBDParticle> CreateParticle(float mass, float damping_factor, std::shared_ptr<Components::Transform> transform);
		void CreateFGRRecord(std::shared_ptr<Components::PBDParticle> p, std::shared_ptr<pbd::BasicGenerator> g);
		void CreateRopeConstraint(std::shared_ptr<Components::PBDParticle> p1, std::shared_ptr<Components::PBDParticle> p2, float ml);
		void ClearContacts();
		void ResolveContact(const Contact& contact);
		void ResolveContacts();
		void UpdatePositions(float t);
		void GeneratorUpdate();
	};
	float Clampf(float v, float max, float min);
	void ClampElementwise(glm::vec3& v, float max, float min);
}
#endif // !PBD_H