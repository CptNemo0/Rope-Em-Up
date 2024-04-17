#include "../../headers/collisions/Collider.h"

#include "../../headers/collisions/CollisionManager.h"

components::Collider::Collider(int layer, int precision, const std::shared_ptr<Mesh> mesh, std::shared_ptr<components::Transform> transform_)
{
	this->transform_ = transform_;
	this->layer_ = layer;
	this->bp_collider_ = collisions::CreateAABB(mesh);
	this->np_collider_ = collisions::CreateConvexHull(precision, mesh);
	collisions::UpdateCentre(bp_collider_, transform_->get_position());
	collisions::UpdateVertices(np_collider_, transform_->get_model_matrix());
}

void components::Collider::Start()
{
}

void components::Collider::Update()
{
	
}

void components::Collider::Destroy()
{
	collisions::CollisionManager::i_->RemoveCollider(shared_from_this());
}

void components::Collider::UpdateColliders()
{
	collisions::UpdateCentre(bp_collider_, transform_->get_position());
	collisions::UpdateVertices(np_collider_, transform_->get_model_matrix());
}

void components::Collider::PredictColliders()
{
	collisions::UpdateCentre(bp_collider_, transform_->get_predicted_position());
	collisions::UpdateVertices(np_collider_, transform_->get_prediction_matrix());
}