#include "../headers/MeshRenderer.h"

Components::MeshRenderer::MeshRenderer(std::shared_ptr<Transform> transform, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader)
{
    this->transform_ = transform;
	this->mesh_ = mesh;
	this->texture_ = texture;
	this->shader_ = shader;
}

void Components::MeshRenderer::Update()
{
    shader_->SetMatrix4("model_matrix", transform_->get_model_matrix());
	glBindTexture(GL_TEXTURE_2D, texture_->id_);
	mesh_->Draw();
}
