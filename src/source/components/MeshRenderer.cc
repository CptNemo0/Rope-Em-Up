#include "../../headers/components/MeshRenderer.h"

components::MeshRenderer::MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader)
{
	this->model_ = model;
	this->shader_ = shader;
}

void components::MeshRenderer::Start()
{
	this->transform_ = this->gameObject_.lock()->transform_;
}

void components::MeshRenderer::Update()
{
    shader_->SetMatrix4("model_matrix", transform_->get_model_matrix());
	model_->Draw(shader_);
}
