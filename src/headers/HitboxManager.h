#ifndef HITBOX_MANAGER_H
#define HITBOX_MANAGER_H

#include <deque>
#include "components/HitboxRenderer.h"
#include "Shader.h"
#include "global.h"
#include "collisions/collisions.h"
class HitboxManager
{
public:
    static HitboxManager* i_;
private:
    HitboxManager();
    ~HitboxManager() = default;

public:

    std::deque<std::shared_ptr<components::HitboxRenderer>> renderers_;
    std::shared_ptr<Shader> shader_;

    

    std::shared_ptr<components::HitboxRenderer> CreateRenderer(glm::vec3 lb, glm::vec3 rb, glm::vec3 rt, glm::vec3 lt);
    void AddRenderer(std::shared_ptr<components::HitboxRenderer> r);
    void RemoveRenderer(std::shared_ptr<components::HitboxRenderer> r);
    void Draw();

    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new HitboxManager();
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

#endif // !HITBOX_MANAGER_H
