#include "../headers/HitboxManager.h"

HitboxManager* HitboxManager::i_ = nullptr;

HitboxManager::HitboxManager()
{
	shader_ = res::get_shader("res/shaders/Hitbox.vert", "res/shaders/Hitbox.frag");
}

std::shared_ptr<components::HitboxRenderer> HitboxManager::CreateRenderer(glm::vec3 lb, glm::vec3 rb, glm::vec3 rt, glm::vec3 lt)
{
	std::shared_ptr<components::HitboxRenderer> renderer = std::make_shared<components::HitboxRenderer>(lb, rb, lt, rt);
	AddRenderer(renderer);
	return renderer;
}

void HitboxManager::AddRenderer(std::shared_ptr<components::HitboxRenderer> r)
{
	renderers_.push_back(r);
}

void HitboxManager::RemoveRenderer(std::shared_ptr<components::HitboxRenderer> r)
{
	auto it = std::find(renderers_.begin(), renderers_.end(), r);
	if (it != renderers_.end())
	{
		renderers_.erase(it);
	}
}

void HitboxManager::Draw()
{
	shader_->Use();
	for (auto& r : renderers_)
	{
		if (r->active_)
		{
			shader_->SetVec4("color_in", r->color_);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, r->texture_->id_);
			shader_->SetInt("albedo", 0);
			r->Draw();
		}
	}
}
