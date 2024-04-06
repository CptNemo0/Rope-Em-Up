#include "../headers/MeshRenderer.h"

Components::MeshRenderer::MeshRenderer(std::shared_ptr<Transform> transform, std::shared_ptr<Model> model,std::shared_ptr<Shader> shader)
{
    this->transform_ = transform;
	this->model_ = model;
	this->shader_ = shader;
}

void Components::MeshRenderer::Update()
{
    shader_->SetMatrix4("model_matrix", transform_->get_model_matrix());
	model_->Draw(shader_);
}
