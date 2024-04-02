#include "../headers/Collider.h"

Components::Collider::Collider(int layer, int precision, const std::shared_ptr<Mesh> mesh, std::shared_ptr<Components::Transform> transform_)
{
	this->transform_ = transform_;
	this->layer_ = layer;
	this->bp_collider_ = collisions::CreateAABB(mesh);
	this->np_collider_ = collisions::CreateConvexHull(precision, mesh);
	collisions::UpdateCentre(bp_collider_, transform_->get_position());
	collisions::UpdateVertices(np_collider_, transform_->get_model_matrix());
}

void Components::Collider::Start()
{
}

void Components::Collider::Update()
{
	collisions::UpdateCentre(bp_collider_, transform_->get_position());
	collisions::UpdateVertices(np_collider_, transform_->get_model_matrix());
}
