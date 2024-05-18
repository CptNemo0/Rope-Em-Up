#ifndef HEALTH_COMPONENT_MANAGER
#define HEALTH_COMPONENT_MANAGER

#include "components/HealthComponent.h"
#include <memory>
#include <deque>

class HealthManager
{
public:
    static HealthManager* i_;
private:
    HealthManager();
    ~HealthManager() = default;

public:
    std::deque<std::shared_ptr<components::HealthComponent>> health_components_;

    std::shared_ptr<components::HealthComponent> CreateHealthComponent(float health, HEALTH_TYPE type);
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
