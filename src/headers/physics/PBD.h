#ifndef PBD_H
#define PBD_H

#include <iostream>
#include <memory>
#include <vector>
#include <deque>

#include "glm/glm.hpp"

#include "../components/Component.h"
#include "../GameObject.h"
#include "../components/Transform.h"

namespace components
{
	class PBDParticle : public Component, public std::enable_shared_from_this<PBDParticle>
	{
	public:
		s_ptr<components::Transform> transform_;
		float mass_;
		float inverse_mass_;
		float damping_factor_;
		glm::vec3 velocity_;
		glm::vec3 forces_;

		bool controllable_ = true;

		PBDParticle(float mass, float damping_factor, s_ptr<components::Transform> transform) : 
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
	// Fixed delta time
	// Right now it's half of actual fixed delta time
	// so that the game doesn't run too fast
	const float kMsPerUpdate = 1.0f / 240.0f;

	class ForceGenerator
	{
	public:
		ForceGenerator() = default;
		~ForceGenerator() = default;
		virtual void GenerateForce(s_ptr<components::PBDParticle> particle) = 0;
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
		void GenerateForce(s_ptr<components::PBDParticle> particle) override;
	};

	struct FGRRecord
	{
		s_ptr<components::PBDParticle> particle;
		s_ptr<ForceGenerator> generator;

		FGRRecord(s_ptr<components::PBDParticle> p, s_ptr<ForceGenerator> g) :
		particle(p), generator(g) {}
		FGRRecord() :
		particle(nullptr), generator(nullptr) {}
		
		~FGRRecord();
		void Generate();
	};

	class Constraint : public std::enable_shared_from_this<Constraint>
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

		RopeConstraint(s_ptr<components::PBDParticle> p1, s_ptr<components::PBDParticle> p2, float ml);
		RopeConstraint() = default;
		~RopeConstraint();
		s_ptr<components::PBDParticle> p1_;
		s_ptr<components::PBDParticle> p2_;
		float max_distance_;

		void Enforce() override;
	};

	class WallConstraint
	{
	public:

		glm::vec3 up_left_;
		glm::vec3 down_right_;
		float offset_;
		WallConstraint(glm::vec3 up_left, glm::vec3 down_right, float offset) :
			up_left_(up_left), down_right_(down_right), offset_(offset){}
		
		WallConstraint() = default;
		~WallConstraint() = default;

		void Enforce(s_ptr<components::PBDParticle> particle);
	};

	struct Contact
	{
		Contact(s_ptr<components::PBDParticle> p1, s_ptr<components::PBDParticle> p2);
		~Contact();
		s_ptr<components::PBDParticle> a;
		s_ptr<components::PBDParticle> b;
		glm::vec3 contact_normal;
	};

	class PBDManager
	{
    public:
        static PBDManager* i_;
    private:
		PBDManager(int it, float coeffiecent_of_restitution, float coeffiecent_of_restitution_wall);
        ~PBDManager() = default;
    public:

		std::deque<s_ptr<components::PBDParticle>> particles_;
		std::vector<pbd::FGRRecord> generator_registry_;
		std::deque<s_ptr<pbd::RopeConstraint>> constraints_;
		std::vector<pbd::Contact> contacts_;

		pbd::WallConstraint walls_;
		int solver_iterations_;
		float coeffiecent_of_restitution_;
		float coeffiecent_of_restitution_wall_;

        static void Initialize(int it, float coeffiecent_of_restitution, float coeffiecent_of_restitution_wall)
        {
            if (i_ == nullptr)
            {
                i_ = new PBDManager(it, coeffiecent_of_restitution, coeffiecent_of_restitution_wall);
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

		void RemoveRecord(s_ptr<ForceGenerator> g);
		void RemoveRecord(s_ptr<components::PBDParticle> p);

		void RemoveConstraint(s_ptr<components::PBDParticle> p);
		void RemoveConstraint(s_ptr<pbd::Constraint> c);

		void RemoveConstraint(pbd::Constraint* c);

		void RemoveParticle(s_ptr<components::PBDParticle> p);

		void Integration(float t);
		
		void ProjectConstraints(float t);

		void set_walls(const WallConstraint& walls)
		{
			walls_ = walls;
		}

		float GetDistanceToClosestWall(s_ptr<components::PBDParticle> p);

		s_ptr<components::PBDParticle> CreateParticle(float mass, float damping_factor, s_ptr<components::Transform> transform);
		void CreateFGRRecord(s_ptr<components::PBDParticle> p, s_ptr<pbd::BasicGenerator> g);
		s_ptr<pbd::RopeConstraint> CreateRopeConstraint(s_ptr<components::PBDParticle> p1, s_ptr<components::PBDParticle> p2, float ml);
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