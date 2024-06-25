#ifndef BILLBOARD_RENDERER_MANAGER_H
#define BILLBOARD_RENDERER_MANAGER_H

#include "components/BillboardRenderer.h"
#include <memory>
#include "Shader.h"
#include "res/Resources.h"
#include "global.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
class BillboardRendererManager
{
public:
    static BillboardRendererManager* i_;
private:
    BillboardRendererManager();
    ~BillboardRendererManager() = default;

public:
    std::vector<std::shared_ptr<components::BillboardRenderer>> renderers_;
    std::shared_ptr<Shader> shader_;

    std::shared_ptr<components::BillboardRenderer> CreateRenderer(s_ptr<tmp::Texture> texture);
    std::shared_ptr<components::BillboardRenderer> CreateRenderer(json &j);
    void AddRenderer(std::shared_ptr<components::BillboardRenderer> r);
    void RemoveRenderer(std::shared_ptr<components::BillboardRenderer> r);
    void UpdateRenderers();
    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new BillboardRendererManager();
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
};

#endif // !BILLBOARD_RENDERER_MANAGER_H
