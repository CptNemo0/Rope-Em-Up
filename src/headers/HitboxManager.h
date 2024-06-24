#ifndef HITBOX_MANAGER_H
#define HITBOX_MANAGER_H

#include <deque>
#include "components/HitboxRenderer.h"
#include "Shader.h"
#include "global.h"
#include "collisions/collisions.h"
#include "collisions/CollisionManager.h"
#include "components/HitboxCollider.h"
#include "components/HealthComponent.h"
#include "components/HpDropComponent.h"
class HitboxManager
{
public:
    static HitboxManager* i_;
private:
    HitboxManager();
    ~HitboxManager() = default;

public:

    std::deque<std::shared_ptr<components::HitboxRenderer>> renderers_;
    std::deque<std::shared_ptr<components::HitboxCollider>> colliders_;
    std::shared_ptr<Shader> shader_;

    std::shared_ptr<components::HitboxRenderer> CreateRenderer(glm::vec3 lb, glm::vec3 rb, glm::vec3 rt, glm::vec3 lt);
    void AddRenderer(std::shared_ptr<components::HitboxRenderer> r);
    void RemoveRenderer(std::shared_ptr<components::HitboxRenderer> r);
    void Draw();

    std::shared_ptr<components::HitboxCollider> CreateCollider(glm::vec3 lb, glm::vec3 rb, glm::vec3 rt, glm::vec3 lt, float dmg);
    void AddCollider(std::shared_ptr<components::HitboxCollider> r);
    void RemoveCollider(std::shared_ptr<components::HitboxCollider> r);
    void CheckCollisions();
    void Check(std::shared_ptr<components::HitboxCollider> hitbox);
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
