#include "../headers/GrassRendererManager.h"

GrassRendererManager* GrassRendererManager::i_ = nullptr;

GrassRendererManager::GrassRendererManager(std::shared_ptr<Model> model)
{
	model_ = model;
}

std::shared_ptr<components::GrassRenderer> GrassRendererManager::CreateRenderer(glm::vec3 ul, glm::vec3 dr, float density)
{
	auto return_value = std::make_shared<components::GrassRenderer>(ul, dr, model_, density);
	AddRenderer(return_value);
	return return_value;
}

std::shared_ptr<components::GrassRenderer> GrassRendererManager::CreateRenderer(float radius, float density)
{
	auto return_value = std::make_shared<components::GrassRenderer>(radius, model_, density);
	AddRenderer(return_value);
	return return_value;
}

std::shared_ptr<components::GrassRenderer> GrassRendererManager::CreateRenderer(json &j)
{
    auto return_value = std::make_shared<components::GrassRenderer>(j);
    AddRenderer(return_value);
    return return_value;
}

void GrassRendererManager::AddRenderer(std::shared_ptr<components::GrassRenderer> r)
{
	renderers_.push_back(r);
}

void GrassRendererManager::RemoveRenderer(std::shared_ptr<components::GrassRenderer> r)
{
	auto it = std::find(renderers_.begin(), renderers_.end(), r);
	if (it != renderers_.end())
	{
		renderers_.erase(it);
	}
}

void GrassRendererManager::Draw(unsigned int id)
{
    for (unsigned int i = 0; i < model_->meshes_[0]->textures_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        string name = model_->meshes_[0]->textures_[i].type_;
        if (name == "texture_albedo")
        {
            glUniform1i(glGetUniformLocation(id, "albedo_map"), i);
            glBindTexture(GL_TEXTURE_2D, model_->meshes_[0]->textures_[i].id_);
        }
        else if (name == "texture_normal")
        {
            glUniform1i(glGetUniformLocation(id, "normal_map"), i);
            glBindTexture(GL_TEXTURE_2D, model_->meshes_[0]->textures_[i].id_);
        }
        else if (name == "texture_metallic")
        {
            glUniform1i(glGetUniformLocation(id, "metallic_map"), i);
            glBindTexture(GL_TEXTURE_2D, model_->meshes_[0]->textures_[i].id_);
        }
        else if (name == "texture_roughness")
        {
            glUniform1i(glGetUniformLocation(id, "roughness_map"), i);
            glBindTexture(GL_TEXTURE_2D, model_->meshes_[0]->textures_[i].id_);
        }
    }

	for (auto r : renderers_)
	{
		if (r->active_)
		{
			r->Draw();
		}
	}
}
