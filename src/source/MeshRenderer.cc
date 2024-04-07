#include "../headers/MeshRenderer.h"

Components::MeshRenderer::MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader)
{
	this->mesh_ = mesh;
	this->texture_ = texture;
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
