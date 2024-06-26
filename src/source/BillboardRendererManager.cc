#include "../headers/BillboardRendererManager.h"

BillboardRendererManager* BillboardRendererManager::i_ = nullptr;

BillboardRendererManager::BillboardRendererManager()
{
	shader_ = res::get_shader("res/shaders/Billboard.vert", "res/shaders/Billboard.geom", "res/shaders/Billboard.frag");
}

std::shared_ptr<components::BillboardRenderer> BillboardRendererManager::CreateRenderer(s_ptr<tmp::Texture> texture)
{
	std::shared_ptr<components::BillboardRenderer> renderer = std::make_shared<components::BillboardRenderer>(texture, shader_);
	AddRenderer(renderer);
	return renderer;
}

std::shared_ptr<components::BillboardRenderer> BillboardRendererManager::CreateRenderer(json &j)
{
	auto texture = res::get_texture(j["texture"]);
    std::shared_ptr<components::BillboardRenderer> renderer = std::make_shared<components::BillboardRenderer>(texture, shader_);
	renderer->position_offset_ = {j["position_offset"][0], j["position_offset"][1], j["position_offset"][2]};
	AddRenderer(renderer);
	return renderer;
}

void BillboardRendererManager::AddRenderer(std::shared_ptr<components::BillboardRenderer> r)
{
	renderers_.push_back(r);
}

void BillboardRendererManager::RemoveRenderer(std::shared_ptr<components::BillboardRenderer> r)
{
	auto it = std::find(renderers_.begin(), renderers_.end(), r);
	if (it != renderers_.end())
	{
		renderers_.erase(it);
	}
}

void BillboardRendererManager::UpdateRenderers()
{
	for (auto& render : renderers_)
	{
		if (render->active_)
		{
			render->Draw();
		}
	}
}
