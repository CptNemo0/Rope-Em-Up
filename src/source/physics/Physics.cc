#include "../../headers/physics/Physics.h"

void physics::LogVec3(glm::vec3 a)
{
	std::cout << "x: " << a.x << " y: " << a.y << " z: " << a.z << "\n";
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


void components::Particle::UpdateAcceleration()
{
	physics::ClampElementwise(forces_, physics::kMaxForce, -physics::kMaxForce);
	acceleration_ = inverse_mass_ * forces_;
}

void components::Particle::UpdateVelocity(float t)
{
	velocity_ = velocity_ + acceleration_ * t;	
}

void components::Particle::UpdatePosition(float t)
{
	velocity_ *= drag_;
	auto new_position = transform_->get_position() + velocity_ * t + 0.5f * acceleration_ * t * t;
	transform_->set_predicted_position(new_position);
}

void components::Particle::AddForce(const glm::vec3& force)
{
	forces_ += force;
}

void components::Particle::ZeroForces()
{
	forces_ = glm::vec3(0.0f);
}

void components::Particle::UpdatePhysics(float t)
{
	assert(t > 0.0f);
	UpdatePosition(t);
	UpdateAcceleration();
	UpdateVelocity(t);
	ZeroForces();
}

void components::Particle::Start()
{
}

void components::Particle::Update()
{
	
}


physics::DragGenerator::DragGenerator(float k1, float k2)
{
	this->k1_ = k1;
	this->k2_ = k2;
}

void physics::DragGenerator::GenerateForce(std::shared_ptr<components::Particle> partilce)
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

void physics::BasicGenerator::GenerateForce(std::shared_ptr<components::Particle> particle)
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
	particles_ = std::vector<std::shared_ptr<components::Particle>>();
}

std::shared_ptr<components::Particle> physics::PhysicsManager::CreateParticle(std::shared_ptr<components::Transform> transform, float mass, float drag)
{
	auto return_value = std::make_shared<components::Particle>(transform, mass, drag);
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

void physics::PhysicsManager::AddFGRRecord(std::shared_ptr<physics::ForceGenerator> generator, std::shared_ptr<components::Particle> particle)
{
	assert(generator != nullptr);
	FGRRecord new_record;
	new_record.generator = generator;
	new_record.particle = particle;
	generator_registry_.push_back(new_record);
}

void physics::PhysicsManager::ResolveContact(physics::Contact& contact)
{
	assert(contact.a != nullptr);
	assert(contact.b != nullptr);

	float separating_velocity = glm::dot((contact.a->velocity_ - contact.b->velocity_), contact.contact_normal);
	if (separating_velocity > 0)
	{
		return;
	}
	
	float new_sep_vel = -separating_velocity * kCoeffiecentOfResitution;
	float delta_velocity = new_sep_vel - separating_velocity;
	
	float total_inverse_mass = contact.a->inverse_mass_ + contact.b->inverse_mass_;

	if (total_inverse_mass <= 0)
	{
		return;
	}

	float impulse = delta_velocity / total_inverse_mass;
	glm::vec3 impulse_per_mass_unit = contact.contact_normal * impulse;

	contact.a->velocity_ += impulse_per_mass_unit * contact.a->inverse_mass_;
	contact.b->velocity_ += -impulse_per_mass_unit * contact.b->inverse_mass_;
}

void physics::PhysicsManager::ResolveContacts(std::vector<physics::Contact> contacts)
{
	for (auto& contact : contacts)
	{
		ResolveContact(contact);
	}
}

void physics::PhysicsManager::RealizePositions()
{
	for (auto& p : particles_)
	{
		p->transform_->set_position(p->transform_->get_predicted_position());
	}
}
	

physics::Contact::Contact(std::shared_ptr<components::Particle> p1, std::shared_ptr<components::Particle> p2)
{
	a = p1;
	b = p2;
	contact_normal = glm::cross(p1->transform_->get_position(), p2->transform_->get_position());
	auto dir = p1->transform_->get_position() - p2->transform_->get_position();
	auto rotation_matrix =  glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 tmp = glm::vec4(dir.x, 0.0f, dir.z, 1.0f);
	tmp = tmp * rotation_matrix;
	contact_normal.x = tmp.x;
	contact_normal.y = tmp.y;
	contact_normal.z = tmp.z;
	glm::normalize(contact_normal);
}
