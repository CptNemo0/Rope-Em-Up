#include "../../headers/physics/PBD.h"

pbd::PBDManager* pbd::PBDManager::i_ = nullptr;

void LogVec3(glm::vec3 a)
{
	cout << "x: " << a.x << " y: " << a.y << " z: " << a.z << "\n";
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

void components::PBDParticle::AddForce(glm::vec3 force)
{
	forces_ += force;
}

void components::PBDParticle::ZeroForces()
{
	forces_ = glm::vec3(0.0f);
}

void components::PBDParticle::SympleticEulerIntegration(float t)
{
	UpdateVelocity(t);
	DampVelocity();
	ZeroForces();
	PredictPosition(t);
}

void components::PBDParticle::DampVelocity()
{
	velocity_ *= (damping_factor_ - (int(friction_) * pbd::kFriction * damping_factor_));
}

void components::PBDParticle::UpdateVelocity(float t)
{
	ClampElementwise(forces_, 3000.0f, -3000.0f);
	velocity_ = (transform_->get_position() - transform_->get_previous_position()) / t + t * inverse_mass_ * forces_;
	velocity_.y = 0.0;
}

void components::PBDParticle::PredictPosition(float t)
{
	transform_->set_predicted_position(transform_->get_position() + t * velocity_);
}

void components::PBDParticle::UpdatePosition(float t)
{
	transform_->set_position(transform_->get_predicted_position());
}

void components::PBDParticle::UpdateRotation(float t)
{
	if (controllable_ && rotate_)
	{
		auto go = gameObject_.lock();
		auto current_forward = go->transform_->get_position() - go->transform_->get_previous_position();
		if (glm::length(current_forward))
		{
			current_forward = glm::normalize(current_forward);
		}
		float angle = glm::degrees(glm::orientedAngle(glm::vec3(0.0f, 0.0f, 1.0f), current_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
		float target_angle = angle + rotation_offset_;
		float current_angle = go->transform_->get_rotation().y;
		   // Normalize angles to the range [0, 360)
		target_angle = fmod(target_angle, 360.0f);
		if (target_angle < 0.0f) target_angle += 360.0f;

		current_angle = fmod(current_angle, 360.0f);
		if (current_angle < 0.0f) current_angle += 360.0f;

		// Calculate the shortest direction to rotate
		float delta_angle = target_angle - current_angle;
		if (delta_angle > 180.0f) delta_angle -= 360.0f;
		if (delta_angle < -180.0f) delta_angle += 360.0f;

		// Interpolate the angle
		float interpolated_angle = current_angle + delta_angle * 20.0f * t;

		// Normalize the interpolated angle to the range [0, 360)
		interpolated_angle = fmod(interpolated_angle, 360.0f);
		if (interpolated_angle < 0.0f) interpolated_angle += 360.0f;

		// Set the rotation
		go->transform_->set_rotation(glm::vec3(0.0f, interpolated_angle, 0.0f));
	}
}

void components::PBDParticle::Start()
{
}

void components::PBDParticle::Update()
{
}

components::PBDParticle::~PBDParticle()
{
	cout << "DELETING PARTICLE!\n";
}

void components::PBDParticle::Destroy()
{
	auto this_shared = shared_from_this();
	pbd::PBDManager::i_->RemoveRecord(this_shared);
	pbd::PBDManager::i_->RemoveParticle(this_shared);
	pbd::PBDManager::i_->RemoveConstraint(this_shared);
}

json components::PBDParticle::Serialize()
{
	json j;

	j["mass"] = mass_;
	j["damping_factor"] = damping_factor_;
	j["rotate"] = rotate_;

	return j;
}

void pbd::BasicGenerator::GenerateForce(s_ptr<components::PBDParticle> particle)
{
	particle->AddForce(direction_ * magnitude_);
}


pbd::FGRRecord::~FGRRecord()
{
	particle = nullptr;
	generator = nullptr;
}

void pbd::FGRRecord::Generate()
{
	generator->GenerateForce(particle);
}


pbd::Contact::Contact(s_ptr<components::PBDParticle> p1, s_ptr<components::PBDParticle> p2)
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
	if (contact_normal.x + contact_normal.y + contact_normal.z != 0)
	{
		contact_normal = glm::normalize(contact_normal);
	}
	
}

pbd::Contact::~Contact()
{
	a = nullptr;
	b = nullptr;
	contact_normal = glm::vec3(0.0f);
}


pbd::RopeConstraint::RopeConstraint(s_ptr<components::PBDParticle> p1, s_ptr<components::PBDParticle> p2, float ml)
{
	p1_ = p1;
	p2_ = p2;
	max_distance_ = ml;
	k_ = 1.0f;
	
}

pbd::RopeConstraint::~RopeConstraint()
{
	
}

void pbd::RopeConstraint::Enforce()
{
	auto x1 = p1_->transform_->get_predicted_position();
	auto x2 = p2_->transform_->get_predicted_position();

	float w1 = p1_->inverse_mass_;
	float w2 = p2_->inverse_mass_;

	float distance = glm::distance(x2, x1);


	if (distance > max_distance_)
	{
		float inv_distance = 1.0f / distance;

		float scale = distance / max_distance_;

		if (scale >= 1.2f)
		{
			scale = 1.0f;
		}
		else
		{
			scale = 0.833f * scale;
		}

		auto separation_vector = (distance - max_distance_) * inv_distance * (x2 - x1);

		auto dx1 = (w1 / (w1 + w2)) * separation_vector;// *scale;
		auto dx2 = -(w2 / (w1 + w2)) * separation_vector;// *scale;

		p1_->transform_->set_predicted_position(x1 + dx1);
		p2_->transform_->set_predicted_position(x2 + dx2);
	}
}


void pbd::WallConstraint::Enforce(s_ptr<components::PBDParticle> particle)
{
	auto pp = particle->transform_->get_predicted_position();

	float max_x = up_left_.x - offset_;
	float min_x = down_right_.x + offset_;
	float max_z = up_left_.z - offset_;
	float min_z = down_right_.z + offset_;

	glm::vec3 diff_x = glm::vec3(0.0f);
	glm::vec3 diff_z = glm::vec3(0.0f);

	if (pp.x > max_x)
	{
		diff_x = glm::vec3(max_x - pp.x, 0.0f, 0.0f);
	}
	
	if (pp.x < min_x)
	{
		diff_x = glm::vec3(min_x - pp.x, 0.0f, 0.0f);
	}

	if (pp.z > max_z)
	{
		diff_x = glm::vec3(0.0f, 0.0f, max_z - pp.z);
	}

	if (pp.z < min_z)
	{
		diff_x = glm::vec3(0.0f, 0.0f, min_z - pp.z);
	}

	pp += 2.0f * pbd::PBDManager::i_->coeffiecent_of_restitution_wall_ * (diff_x + diff_z);
	particle->transform_->set_predicted_position(pp);
}


pbd::PBDManager::PBDManager(PBDManagerInitStruct& init)
{
	particles_ = std::deque<s_ptr<components::PBDParticle>>();
	generator_registry_ = std::vector<pbd::FGRRecord>();
	constraints_ = std::deque<s_ptr<pbd::RopeConstraint>>();
	contacts_ = std::vector<pbd::Contact>();
	solver_iterations_ = init.it;
	coeffiecent_of_restitution_ = init.coeffiecent_of_restitution;
	coeffiecent_of_restitution_wall_ = init.coeffiecent_of_restitution_wall;
}

void pbd::PBDManager::RemoveRecord(s_ptr<ForceGenerator> g)
{
	auto it = std::find_if(generator_registry_.begin(), generator_registry_.end(), [g](const pbd::FGRRecord &record)
    {
        return record.generator == g;
    });
    if (it != generator_registry_.end())
    {
		generator_registry_[it - generator_registry_.begin()].~FGRRecord();;
        generator_registry_.erase(it);
    }
}

void pbd::PBDManager::RemoveRecord(s_ptr<components::PBDParticle> p)
{
	auto it = std::find_if(generator_registry_.begin(), generator_registry_.end(), [p](const pbd::FGRRecord &record)
    {
        return record.particle == p;
    });
    if (it != generator_registry_.end())
    {
		generator_registry_[it - generator_registry_.begin()].~FGRRecord();;
        generator_registry_.erase(it);
    }
}

void pbd::PBDManager::RemoveConstraint(s_ptr<components::PBDParticle> p)
{
	std::erase_if(pbd::PBDManager::i_->constraints_, [p](const s_ptr<pbd::RopeConstraint> &A)
	{
		return A->p1_ == p || A->p2_ == p;
	});
}

void pbd::PBDManager::RemoveConstraint(s_ptr<pbd::Constraint> c)
{
	auto it = std::find(constraints_.begin(), constraints_.end(), c);
	if (it != constraints_.end())
	{
		constraints_.erase(it);
	}
}

void pbd::PBDManager::RemoveConstraint(pbd::Constraint* c)
{
	int pos = -1;
	for (int i = 0; i < constraints_.size(); i++)
	{
		if (constraints_[i].get() == c)
		{
			pos = i;
			break;
		}
	}

	if (pos != -1)
	{
		constraints_.erase(constraints_.begin() + pos);
	}
}

void  pbd::PBDManager::RemoveParticle(s_ptr<components::PBDParticle> p)
{
	auto it = std::find(particles_.begin(), particles_.end(), p);
	if (it != particles_.end())
	{
		particles_.erase(it);
	}
}

void pbd::PBDManager::Integration(float t)
{
	for (auto& p : particles_)
	{
		if (p->active_)
		{
			p->SympleticEulerIntegration(t);
		}
	}
}

void pbd::PBDManager::ProjectConstraints(float t)
{
	for (int i = 0; i < solver_iterations_; i++)
	{
		for (auto& c : constraints_)
		{
			if (c->p1_->active_ && c->p2_->active_)
			{
				c->Enforce();
			}
		}
	}

	
	for (auto& particle : particles_)
	{
		if (particle->active_)
		{
			walls_.Enforce(particle);
		}
	}
}

float pbd::PBDManager::GetDistanceToClosestWall(s_ptr<components::PBDParticle> particle)
{
	float return_value;

	auto pp = particle->transform_->get_predicted_position();

	float max_x = walls_.down_right_.x - walls_.offset_;
	float min_x = walls_.up_left_.x + walls_.offset_;
	float max_z = walls_.up_left_.z - walls_.offset_;
	float min_z = walls_.down_right_.z + walls_.offset_;

	float diff_x = 0.0f;
	float diff_z = 0.0f;

	if (pp.x > max_x)
	{
		diff_x = std::fabs(max_x - pp.x);
	}

	if (pp.x < min_x)
	{
		diff_x = std::fabs(min_x - pp.x);
	}

	if (pp.z > max_z)
	{
		diff_x = std::fabs(max_z - pp.z);
	}

	if (pp.z < min_z)
	{
		diff_x = std::fabs(min_z - pp.z);
	}

	return std::max(diff_x, diff_z);
}

s_ptr<components::PBDParticle> pbd::PBDManager::CreateParticle(float mass, float damping_factor, s_ptr<components::Transform> transform, bool rotate)
{
	auto p = make_shared<components::PBDParticle>(mass, damping_factor, transform, rotate);
	particles_.push_back(p);
	
	return p;
}

s_ptr<components::PBDParticle> pbd::PBDManager::CreateParticle(json &j, s_ptr<GameObject> go)
{
	float mass = j["mass"];
	float damping_factor = j["damping_factor"];
	bool rotate = j["rotate"];
	auto transform = go->transform_;
	auto p = make_shared<components::PBDParticle>(mass, damping_factor, transform, rotate);
	particles_.push_back(p);
	
	return p;
}

void pbd::PBDManager::CreateFGRRecord(s_ptr<components::PBDParticle> p, s_ptr<pbd::BasicGenerator> g)
{
	pbd::FGRRecord fgrr = pbd::FGRRecord(p, g);
	generator_registry_.push_back(fgrr);
}

s_ptr<pbd::RopeConstraint> pbd::PBDManager::CreateRopeConstraint(s_ptr<components::PBDParticle> p1, s_ptr<components::PBDParticle> p2, float ml)
{
	auto constraint = std::make_shared<RopeConstraint>(p1, p2, ml);
	constraints_.push_back(constraint);
	return constraint;
}

void pbd::PBDManager::ClearContacts()
{
	for (auto& c : contacts_)
	{
		c.~Contact();
	}
	contacts_.clear();

	for (auto& p : particles_)
	{
		p->friction_ = false;
	}
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
		if (p->active_)
		{
			p->UpdatePosition(t);
		}
	}
}

void pbd::PBDManager::UpdateRotations(float t)
{
	for (auto& p : particles_)
	{
		if (p->active_)
		{
			p->UpdateRotation(t);
		}
	}
}

void pbd::PBDManager::GeneratorUpdate()
{
	for (auto& record : generator_registry_)
	{
		if (record.particle->active_)
		{
			record.Generate();
		}
	}
}
