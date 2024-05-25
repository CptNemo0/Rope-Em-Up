#include "../../headers/components/MeshRenderer.h"

components::MeshRenderer::MeshRenderer(s_ptr<Model> model, s_ptr<Shader> shader)
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
	if (model_->m_BoneCounter > 0)
	{
		shader_->SetBool("useBones", true);
	}
	else
	{
		shader_->SetBool("useBones", false);
	}
	model_->Draw(shader_);
}

void components::MeshRenderer::Render(s_ptr<Shader> shader)
{
	shader->SetMatrix4("model_matrix", transform_->get_model_matrix());
	model_->Draw(shader);
}
