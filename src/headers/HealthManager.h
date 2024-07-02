#ifndef HEALTH_COMPONENT_MANAGER
#define HEALTH_COMPONENT_MANAGER

#include <memory>
#include <deque>
#include <set>
#include "nlohmann/json.hpp"

#include "components/HealthComponent.h"
#include "typedef.h"
#include "glm/glm.hpp"

#include "glad/glad.h"
#include "Model.h"
#include "Shader.h"

class HealthManager
{
public:
    static HealthManager* i_;
private:
    HealthManager();
    ~HealthManager() = default;

public:
    std::deque<std::shared_ptr<components::HealthComponent>> health_components_;
    std::deque<std::shared_ptr<components::HealthComponent>> dead_;
    bool something_died_;
    glm::vec3 where_;
    std::set<HEALTH_TYPE> what_;
    std::shared_ptr<Model> shield_model_;
    std::shared_ptr<Shader> shield_shader_;

    std::shared_ptr<components::HealthComponent> CreateHealthComponent(float health, HEALTH_TYPE type);
    std::shared_ptr<components::HealthComponent> CreateHealthComponent(json &j);
    void AddHealthComponent(std::shared_ptr<components::HealthComponent> h);
    void RemoveHealthComponent(std::shared_ptr<components::HealthComponent> h);

    void DeathUpdate();
    void ManageDeath(s_ptr<GameObject> go);

    void DrawShields();

    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new HealthManager();
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

#endif // !HEALTH_COMPONENT_MANAGER
