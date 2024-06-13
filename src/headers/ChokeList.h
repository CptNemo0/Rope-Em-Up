#ifndef CHOKE_LIST_H
#define CHOKE_LIST_H

#include "HealthManager.h"
#include "components/HealthComponent.h"

class ChokeList
{
public:
    static ChokeList* i_;
private:
    ChokeList();
    ~ChokeList() = default;

public:
    std::vector<std::shared_ptr<components::HealthComponent>> health_components_;

    void AddHealthComponent(std::shared_ptr<components::HealthComponent> h);
    void RemoveHealthComponent(std::shared_ptr<components::HealthComponent> h);
    std::unordered_set<HEALTH_TYPE> Choke(float dmg);
    void RemoveDead();

    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new ChokeList();
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

#endif // !CHOKE_LIST_H
