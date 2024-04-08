#include "../headers/MeshRenderer.h"

Components::MeshRenderer::MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader)
{
	this->model_ = model;
	this->mesh_ = model_->meshes_.at(0);
	this->shader_ = shader;
}

void Components::MeshRenderer::Start()
{
	this->transform_ = this->gameObject_.lock()->transform_;
}

void Components::MeshRenderer::Update()
{
    shader_->SetMatrix4("model_matrix", transform_->get_model_matrix());
	model_->Draw(shader_);
}
