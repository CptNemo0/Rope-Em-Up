#include "../headers/Physics.h"

void LogVec3(glm::vec3 a)
{
	std::cout << a.x << " " << a.y << " " << a.z << "\n";
}

void physics::Particle::UpdateAcceleration()
{
	acceleration_ = inverse_mass_ * forces_;
}

void physics::Particle::UpdateVelocity(float t)
{
	velocity_ = velocity_ + acceleration_ * t;
}

void physics::Particle::UpdatePosition(float t)
{
	auto new_position = transform_->get_position() + velocity_ * t + 0.5f * acceleration_ * t * t;
	transform_->set_position(new_position);
}

void physics::Particle::AddForce(glm::vec3 force)
{
	forces_ += force;
}

void physics::Particle::ZeroForces()
{
	forces_ = glm::vec3(0.0f);
}

void physics::Particle::UpdatePhysics(float t)
{
	assert(t > 0.0f);
	UpdatePosition(t);
	UpdateAcceleration();
	UpdateVelocity(t);
	ZeroForces();
	/*std::cout << "acc: ";
	LogVec3(acceleration_);
	std::cout << "vel: ";
	LogVec3(velocity_);
	std::cout << "pos: ";
	LogVec3(transform_->get_position());*/
}

physics::DragGenerator::DragGenerator(float k1, float k2)
{
	this->k1_ = k1;
	this->k2_ = k2;
}

void physics::DragGenerator::GenerateForce(std::shared_ptr<Particle> partilce)
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

void physics::BasicGenerator::GenerateForce(std::shared_ptr<Particle> particle)
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
	common_drag_generator_ = std::make_shared<DragGenerator>(10.0f, 20.0f);
	generator_registry_ = std::vector<FGRRecord>();
	particles_ = std::vector<std::shared_ptr<Particle>>();
}

std::shared_ptr<physics::Particle> physics::PhysicsManager::CreateParticle(std::shared_ptr<Components::Transform> transform, float mass)
{
	auto return_value = std::make_shared<physics::Particle>(transform, mass);
	FGRRecord new_record;
	new_record.generator = common_drag_generator_;
	new_record.particle = return_value;
	generator_registry_.push_back(new_record);
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

void physics::PhysicsManager::AddFGRRecord(std::shared_ptr<physics::ForceGenerator> generator, std::shared_ptr<physics::Particle> particle)
{
	assert(generator != nullptr);
	FGRRecord new_record;
	new_record.generator = generator;
	new_record.particle = particle;
	generator_registry_.push_back(new_record);
}
