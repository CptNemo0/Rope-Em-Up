#include "../headers/Collider.h"

Components::Collider::Collider(int layer, int precision, const std::shared_ptr<Mesh> mesh)
{
	this->layer = layer;
	this->bp_collider_ = collisions::CreateAABB(mesh);
	this->np_collider_ = collisions::CreateConvexHull(precision, mesh);
	collisions::UpdateCentre(bp_collider_, game_object_->transform->get_model_matrix());
	collisions::UpdateVertices(np_collider_, game_object_->transform_->get_model_matrix());
}

void Components::Collider::Start()
{
}

void Components::Collider::Update()
{
	collisions::UpdateCentre(bp_collider_, game_object_->transform_->get_position());
	collisions::UpdateVertices(np_collider_, game_object_->transform_->get_model_matrix());
}
