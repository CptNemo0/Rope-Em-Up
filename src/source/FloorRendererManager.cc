#include "../headers/FloorRendererManager.h"

FloorRendererManager* FloorRendererManager::i_ = nullptr;


FloorRendererManager::FloorRendererManager(std::shared_ptr<Shader> shader)
{
	shader_ = shader;
    floor_height_texture_ = res::get_texture("res/models/enviroment/floor/floor_height.png");
    floor_normal_texture_ = res::get_texture("res/models/enviroment/floor/floor_normal.png");
    floor_albedo_texture_ = res::get_texture("res/models/enviroment/floor/floor_albedo1.png");
    floor_roughness_texture_ = res::get_texture("res/models/enviroment/floor/floor_roughness.png");
}

std::shared_ptr<components::FloorRenderer> FloorRendererManager::CreateFloorRenderer()
{
    auto renderer = std::make_shared<components::FloorRenderer>();
    AddRenderer(renderer);
    return renderer;
}

void FloorRendererManager::AddRenderer(std::shared_ptr<components::FloorRenderer> renderer)
{
    renderers_.push_back(renderer);
}

void FloorRendererManager::RemoveRenderer(std::shared_ptr<components::FloorRenderer> r)
{
    auto it = std::find(renderers_.begin(), renderers_.end(), r);
    if (it != renderers_.end())
    {
        renderers_.erase(it);
    }
}

void FloorRendererManager::Draw()
{
	shader_->Use();

    shader_->SetInt("height_map", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floor_height_texture_->id_);

    shader_->SetInt("albedo_map", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, floor_albedo_texture_->id_);

    shader_->SetInt("normal_map", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, floor_normal_texture_->id_);

    shader_->SetInt("roughness_map", 3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, floor_roughness_texture_->id_);

    shader_->SetMatrix4("view_matrix", Global::i_->active_camera_->GetViewMatrix());
    
	for (auto r : renderers_)
	{
        if (r->active_)
        {
            r->Draw(shader_);
        }	
	}
}
