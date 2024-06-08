#ifndef HEALTH_COMPONENT_MANAGER
#define HEALTH_COMPONENT_MANAGER

#include <memory>
#include <deque>
#include "nlohmann/json.hpp"

#include "components/HealthComponent.h"
#include "typedef.h"
#include "glm/glm.hpp"

class HealthManager
{
public:
    static HealthManager* i_;
private:
    HealthManager();
    ~HealthManager() = default;

public:
    std::deque<std::shared_ptr<components::HealthComponent>> health_components_;
    bool something_died_;
    glm::vec3 where_;
    HEALTH_TYPE what_;

    std::shared_ptr<components::HealthComponent> CreateHealthComponent(float health, HEALTH_TYPE type);
    std::shared_ptr<components::HealthComponent> CreateHealthComponent(json &j);
    void AddHealthComponent(std::shared_ptr<components::HealthComponent> h);
    void RemoveHealthComponent(std::shared_ptr<components::HealthComponent> h);

    void DeathUpdate();

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
