#include "../headers/PBD.h"

pbd::PBDManager* pbd::PBDManager::i_ = nullptr;

void LogVec3(glm::vec3 a)
{
	std::cout << "x: " << a.x << " y: " << a.y << " z: " << a.z << "\n";
}

float pbd::Clampf(float v, float max, float min)
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

void ClampElementwise(glm::vec3& v, float max, float min)
{
	v.x = pbd::Clampf(v.x, max, min);
	v.y = pbd::Clampf(v.y, max, min);
	v.z = pbd::Clampf(v.z, max, min);
}

void Components::PBDParticle::AddForce(glm::vec3 force)
{
	forces_ += force;
}

void Components::PBDParticle::ZeroForces()
{
	forces_ = glm::vec3(0.0f);
}

void Components::PBDParticle::SympleticEulerIntegration(float t)
{
	UpdateVelocity(t);
	DampVelocity();
	ZeroForces();
	PredictPosition(t);
}

void Components::PBDParticle::DampVelocity()
{
	velocity_ *= damping_factor_;
}

void Components::PBDParticle::UpdateVelocity(float t)
{
	ClampElementwise(forces_, 1000.0f, -1000.0f);
	//velocity_ = velocity_ + t * inverse_mass_ * forces_;
	velocity_ = (transform_->get_position() - transform_->get_previous_position()) / t + t * inverse_mass_ * forces_;
}

void Components::PBDParticle::PredictPosition(float t)
{
	transform_->set_predicted_position(transform_->get_position() + t * velocity_);
}

void Components::PBDParticle::UpdatePosition(float t)
{
	if (glm::length(velocity_) != 0.0f && transform_->get_position() == transform_->get_predicted_position())
	{
		transform_->set_position(transform_->get_predicted_position());
	}
	else
	{
		auto pp = transform_->get_predicted_position();
		auto p = transform_->get_position();
		//velocity_ = (pp - p) / t;
		transform_->set_position(transform_->get_predicted_position());
	}
	
}

void Components::PBDParticle::Start()
{
}

void Components::PBDParticle::Update()
{
}

void Components::PBDParticle::Destroy()
{
	auto &vec = pbd::PBDManager::i_->constraints_;
	auto this_shared = shared_from_this();
	auto it = std::find_if(vec.begin(), vec.end(), [this_shared](const pbd::RopeConstraint &constraint)
	{
		return constraint.p1_ == this_shared || constraint.p2_ == this_shared;
	});
	if (it != vec.end())
	{
		vec.erase(it);
	}
}

Components::PBDParticle::~PBDParticle()
{
	std::cout << "DELETING PARTICLE!\n";
}


void pbd::BasicGenerator::GenerateForce(std::shared_ptr<Components::PBDParticle> particle)
{
	particle->AddForce(direction_ * magnitude_);
}


void pbd::FGRRecord::Generate()
{
	generator->GenerateForce(particle);
}


pbd::Contact::Contact(std::shared_ptr<Components::PBDParticle> p1, std::shared_ptr<Components::PBDParticle> p2)
{
	a = p1;
	b = p2;
	contact_normal = glm::cross(p1->transform_->get_position(), p2->transform_->get_position());
	auto dir = p1->transform_->get_position() - p2->transform_->get_position();
	auto rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 tmp = glm::vec4(dir.x, 0.0f, dir.z, 1.0f);
	tmp = tmp * rotation_matrix;
	contact_normal.x = tmp.x;
	contact_normal.y = tmp.y;
	contact_normal.z = tmp.z;
	glm::normalize(contact_normal);
}

pbd::Contact::~Contact()
{
	a = nullptr;
	b = nullptr;
	contact_normal = glm::vec3(0.0f);
}


pbd::RopeConstraint::RopeConstraint(std::shared_ptr<Components::PBDParticle> p1, std::shared_ptr<Components::PBDParticle> p2, float ml)
{
	p1_ = p1;
	p2_ = p2;
	max_distance_ = ml;
	k_ = 1.0f;
}

void pbd::RopeConstraint::Enforce()
{
	auto x1 = p1_->transform_->get_predicted_position();
	auto x2 = p2_->transform_->get_predicted_position();

	float w1 = p1_->inverse_mass_;
	float w2 = p2_->inverse_mass_;

	float distance = glm::distance(x2, x1);
	
	/*if (distance > max_distance_)
	{
		float inv_distance = 1.0f / distance;

		auto separation_vector = (distance - max_distance_) * inv_distance * (x2 - x1);

		auto dx1 = 0.5f * separation_vector;
		auto dx2 = -0.5f * separation_vector;

		p1_->transform_->set_predicted_position(x1 + dx1);
		p2_->transform_->set_predicted_position(x2 + dx2);
	}*/

	if (distance > max_distance_)
	{
		float inv_distance = 1.0f / distance;

		auto separation_vector = (distance - max_distance_) * inv_distance * (x2 - x1);

		auto dx1 = (w1 / (w1 + w2)) * separation_vector;
		auto dx2 = -(w2 / (w1 + w2)) * separation_vector;

		p1_->transform_->set_predicted_position(x1 + dx1);
		p2_->transform_->set_predicted_position(x2 + dx2);
	}
}


pbd::PBDManager::PBDManager(int it, float coeffiecent_of_restitution)
{
	particles_ = std::vector<std::shared_ptr<Components::PBDParticle>>();
	generator_registry_ = std::vector<pbd::FGRRecord>();
	constraints_ = std::vector<pbd::RopeConstraint>();
	contacts_ = std::vector<pbd::Contact>();
	solver_iterations_ = it;
	coeffiecent_of_restitution_ = coeffiecent_of_restitution;
}

void pbd::PBDManager::RemoveRecord(std::shared_ptr<ForceGenerator> g)
{
	auto it = std::find_if(generator_registry_.begin(), generator_registry_.end(), [g](const pbd::FGRRecord &record)
    {
        return record.generator == g;
    });
    if (it != generator_registry_.end())
    {
        generator_registry_.erase(it);
    }
}

void pbd::PBDManager::RemoveRecord(std::shared_ptr<Components::PBDParticle> p)
{
	auto it = std::find_if(generator_registry_.begin(), generator_registry_.end(), [p](const pbd::FGRRecord &record)
    {
        return record.particle == p;
    });
    if (it != generator_registry_.end())
    {
        generator_registry_.erase(it);
    }
}

void pbd::PBDManager::Integration(float t)
{
	for (auto& p : particles_)
	{
		p->SympleticEulerIntegration(t);
	}
}

void pbd::PBDManager::ProjectConstraints(float t)
{
	for (int i = 0; i < solver_iterations_; i++)
	{
		for (auto& c : constraints_)
		{
			c.Enforce();
		}
	}
}

std::shared_ptr<Components::PBDParticle> pbd::PBDManager::CreateParticle(float mass, float damping_factor, std::shared_ptr<Components::Transform> transform)
{
	auto p = std::make_shared<Components::PBDParticle>(mass, damping_factor, transform);
	particles_.push_back(p);
	return p;
}

void pbd::PBDManager::CreateFGRRecord(std::shared_ptr<Components::PBDParticle> p, std::shared_ptr<pbd::BasicGenerator> g)
{
	pbd::FGRRecord fgrr = pbd::FGRRecord(p, g);
	generator_registry_.push_back(fgrr);
}

void pbd::PBDManager::CreateRopeConstraint(std::shared_ptr<Components::PBDParticle> p1, std::shared_ptr<Components::PBDParticle> p2, float ml)
{
	RopeConstraint constraint = RopeConstraint(p1, p2, ml);
	constraints_.push_back(constraint);
}

void pbd::PBDManager::ClearContacts()
{
	for (auto& c : contacts_)
	{
		c.~Contact();
	}
	contacts_.clear();
}

void pbd::PBDManager::ResolveContact(const Contact& contact)
{
	assert(contact.a != nullptr);
	assert(contact.b != nullptr);

	float separating_velocity = glm::dot((contact.a->velocity_ - contact.b->velocity_), contact.contact_normal);
	
	if (separating_velocity > 0)
	{
		return;
	}

	float new_sep_vel = -separating_velocity * coeffiecent_of_restitution_;
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

void pbd::PBDManager::ResolveContacts()
{
	for (auto& c : contacts_)
	{
		ResolveContact(c);
	}
}

void pbd::PBDManager::UpdatePositions(float t)
{
	for (auto& p : particles_)
	{
		p->UpdatePosition(t);
	}
}

void pbd::PBDManager::GeneratorUpdate()
{
	for (auto& record : generator_registry_)
	{
		record.Generate();
	}
}
