#include "../headers/MeshRenderer.h"

Components::MeshRenderer::MeshRenderer(std::shared_ptr<Transform> transform, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::shared_ptr<Shader> shader)
{
    this->transform_ = transform;
	this->mesh_ = mesh;
	this->material_ = material;
	this->shader_ = shader;
}

void Components::MeshRenderer::Update()
{
    shader_->SetMatrix4("model_matrix", transform_->get_model_matrix());
	for ( unsigned int i = 0; i < material_->textures.size(); i++)
	{
		glUniform1i(glGetUniformLocation(shader_->id_, material_->textures.at(i).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, material_->id_);

	}
	mesh_->Draw();
}
