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

components::MeshRenderer::MeshRenderer(json &j)
{
	auto model = res::get_model(j["model_path"]);
	auto shader = res::get_shader(j["shader_paths"][0], j["shader_paths"][1]);
	this->MeshRenderer::MeshRenderer(model, shader);
}

json components::MeshRenderer::Serialize()
{
    json j;

	j["model_path"] = model_->path_;
	j["shader_paths"] = {shader_->v_path, shader_->f_path};

	return j;
}
