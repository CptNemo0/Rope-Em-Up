#include "../headers/Physics.h"

void physics::Particle::UpdateAcceleration()
{
	acceleration_ = inverse_mass_ * forces_;
}

void physics::Particle::UpdateVelocity(float t)
{
	velocity_ = acceleration_ * t;
}

void physics::Particle::UpdatePosition(float t)
{
	auto new_position = transform_->get_position() + velocity_ * t + 0.5f * acceleration_ * acceleration_;
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