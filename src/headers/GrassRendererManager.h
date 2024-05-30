#ifndef GRASS_RENDERER_MANAGER
#define GRASS_RENDERER_MANAGER

#include "components/GrassRenderer.h"
#include <vector>
#include <memory>

class GrassRendererManager
{
public:
    static GrassRendererManager* i_;
private:
    GrassRendererManager(std::shared_ptr<Model> model);
    ~GrassRendererManager() = default;

public:

    static void Initialize(std::shared_ptr<Model> model)
    {
        if (i_ == nullptr)
        {
            i_ = new GrassRendererManager(model);
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

    std::vector<std::shared_ptr<components::GrassRenderer>> renderers_;
    std::shared_ptr<Model> model_;

    std::shared_ptr<components::GrassRenderer> CreateRenderer(glm::vec3 ul, glm::vec3 dr, float density);
    std::shared_ptr<components::GrassRenderer> CreateRenderer(float radius,  float density);

    void AddRenderer(std::shared_ptr<components::GrassRenderer>);
    void RemoveRenderer(std::shared_ptr<components::GrassRenderer>);

    void Draw();
};


#endif // !GRASS_RENDERER_MANAGER
