#include "../../headers/collisions/Collider.h"

#include "../../headers/collisions/CollisionManager.h"
#include "../../headers/GameObject.h"

components::Collider::Collider(int layer, int precision, const s_ptr<Model> model, int mesh_index, s_ptr<components::Transform> transform_)
{
	auto mesh = model->meshes_[mesh_index];
	this->transform_ = transform_;
	this->layer_ = layer;
	this->precision_ = precision;
	this->model_ = model;
	this->mesh_index_ = mesh_index;
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
	cout << "COLLIDER DESTROY" << endl;
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

components::Collider::Collider(json &j, s_ptr<GameObject> go)
{
	int layer = j["layer"];
	int precision = j["precision"];
	int mesh_index = j["mesh_index"];
	string model_path = j["model_path"];

	this->Collider::Collider(layer, precision, res::get_model(model_path), mesh_index, go->transform_);
}

json components::Collider::Serialize()
{
    json j;

	j["layer"] = layer_;
	j["precision"] = precision_;
	j["mesh_index"] = mesh_index_;
	j["model_path"] = model_->path_;

	return j;
}
