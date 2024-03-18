#include "../headers/GameObject.h"

GameObject::GameObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader)
	: Transform::Transform()
{
	this->mesh_ = mesh;
	this->texture_ = texture;
	this->shader_ = shader;
}

void GameObject::Render()
{
	shader_->SetMatrix4("model_matrix", get_model_matrix());
	glBindTexture(GL_TEXTURE_2D, texture_->id_);
	mesh_->Draw();
}
