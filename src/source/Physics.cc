#include "../headers/Physics.h"

void physics::LogVec3(glm::vec3 a)
{
	std::cout << a.x << " " << a.y << " " << a.z << "\n";
}

float physics::Clampf(float v, float max, float min)
{
	if (v < min)
	{
		return min;
	}
	else if (v > max)
	{
		return max;
	}

	return v;
}

void physics::ClampElementwise(glm::vec3& v, float max, float min)
{
	v.x = Clampf(v.x, max, min);
	v.y = Clampf(v.y, max, min);
	v.z = Clampf(v.z, max, min);
}

void physics::Sigmoid(glm::vec3& v)
{
	v.x = std::exp2f(-v.x);
	v.y = 0.0f;
	v.z = std::exp2f(-v.z);
}

void Components::Particle::UpdateAcceleration()
{
	physics::ClampElementwise(forces_, physics::kMaxForce, -physics::kMaxForce);
	acceleration_ = inverse_mass_ * forces_;
}

void Components::Particle::UpdateVelocity(float t)
{
	velocity_ = velocity_ + acceleration_ * t;	
}

void Components::Particle::UpdatePosition(float t)
{
	velocity_ *= drag_;
	auto new_position = transform_->get_position() + velocity_ * t + 0.5f * acceleration_ * t * t;
	transform_->set_position(new_position);
}

void Components::Particle::AddForce(const glm::vec3& force)
{
	forces_ += force;
}

void Components::Particle::ZeroForces()
{
	forces_ = glm::vec3(0.0f);
}

void Components::Particle::UpdatePhysics(float t)
{
	assert(t > 0.0f);
	UpdatePosition(t);
	UpdateAcceleration();
	UpdateVelocity(t);
	ZeroForces();
}

void Components::Particle::Start()
{
}

void Components::Particle::Update()
{
}

physics::DragGenerator::DragGenerator(float k1, float k2)
{
	this->k1_ = k1;
	this->k2_ = k2;
}

void physics::DragGenerator::GenerateForce(std::shared_ptr<Components::Particle> partilce)
{
	auto force = partilce->velocity_;

	float drag_coeffiecent = glm::length(force);
	drag_coeffiecent = k1_ * drag_coeffiecent + k2_ * drag_coeffiecent * drag_coeffiecent;

	glm::normalize(force);

	force *= -drag_coeffiecent;
	partilce->AddForce(force);
}

physics::BasicGenerator::BasicGenerator()
{
	direction_ = glm::vec3(0.0f);
	magnitude_ = 0.0f;
}

void physics::BasicGenerator::GenerateForce(std::shared_ptr<Components::Particle> particle)
{
	particle->AddForce(direction_ * magnitude_);
}

void physics::FGRRecord::Generate()
{
	generator->GenerateForce(particle);
}

physics::PhysicsManager* physics::PhysicsManager::i_ = nullptr;

physics::PhysicsManager::PhysicsManager()
{
	generator_registry_ = std::vector<FGRRecord>();
	particles_ = std::vector<std::shared_ptr<Components::Particle>>();
}

std::shared_ptr<Components::Particle> physics::PhysicsManager::CreateParticle(std::shared_ptr<Components::Transform> transform, float mass, float drag)
{
	auto return_value = std::make_shared<Components::Particle>(transform, mass, drag);
	particles_.push_back(return_value);
	return return_value;
}

void physics::PhysicsManager::GeneratorUpdate()
{
	for (auto& record : generator_registry_)
	{
		record.Generate();
	}
}

void physics::PhysicsManager::ParticleUpdate(float t)
{
	for (auto& particle : particles_)
	{
		particle->UpdatePhysics(t);
	}
}

void physics::PhysicsManager::AddFGRRecord(std::shared_ptr<physics::ForceGenerator> generator, std::shared_ptr<Components::Particle> particle)
{
	assert(generator != nullptr);
	FGRRecord new_record;
	new_record.generator = generator;
	new_record.particle = particle;
	generator_registry_.push_back(new_record);
}

void physics::PhysicsManager::ResolveContact(physics::Contact& contact)
{
	glm::vec3 va = glm::vec3(0.0f);
	glm::vec3 vb = glm::vec3(0.0f);

	glm::vec3 ua = contact.a->velocity_;
	glm::vec3 ub = contact.b->velocity_;

	float ma = contact.a->mass_;
	float mb = contact.b->mass_;

	float mass_sum = ma + mb;
	assert(mass_sum > 0.00001f);
	glm::vec3 mumu = ma * ua + mb * ub;

	va = ( (mumu + (0.1f * mb * (ub - ua))) / (mass_sum));
	vb = ( (mumu + (0.1f * ma * (ua - ub))) / (mass_sum));

	contact.a->velocity_ = va;
	contact.b->velocity_ = vb;
}

void physics::PhysicsManager::ResolveContacts(std::vector<physics::Contact> contacts)
{
	for (auto& contact : contacts)
	{
		ResolveContact(contact);
	}
}
