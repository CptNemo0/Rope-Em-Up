#ifndef FLOOR_RENDERER_MANAGER_H
#define FLOOR_RENDERER_MANAGER_H

#include <memory>
#include "Shader.h"
#include "components/FloorRenderer.h"
#include <deque>
#include "global.h"

class FloorRendererManager
{
public:
    static FloorRendererManager* i_;
private:
    FloorRendererManager(std::shared_ptr<Shader> shader);
    ~FloorRendererManager() = default;

public:

    std::deque<std::shared_ptr<components::FloorRenderer>> renderers_;
    std::shared_ptr<Shader> shader_;
    std::shared_ptr<tmp::Texture> floor_height_texture_;
    std::shared_ptr<tmp::Texture> floor_normal_texture_;
    std::shared_ptr<tmp::Texture> floor_albedo_texture_;
    std::shared_ptr<tmp::Texture> floor_roughness_texture_;

    static void Initialize(std::shared_ptr<Shader> shader)
    {
        if (i_ == nullptr)
        {
            i_ = new FloorRendererManager(shader);
            components::FloorRenderer::Init();
        }
    }

    static void Destroy()
    {
        if (i_ != nullptr)
        {
            delete i_;
            i_ = nullptr;
        }
    }

    std::shared_ptr<components::FloorRenderer> CreateFloorRenderer();
    void AddRenderer(std::shared_ptr<components::FloorRenderer> renderer);
    void RemoveRenderer(std::shared_ptr<components::FloorRenderer> renderer);
    void Draw();
};

#endif // !FLOOR_RENDERER_MANAGER_H
