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

void GrassRendererManager::Draw()
{
	for (auto r : renderers_)
	{
		if (r->active_)
		{
			r->Draw();
		}
	}
}
