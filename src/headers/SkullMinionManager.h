#ifndef SKULL_MINION_MANAGER_H
#define SKULL_MINION_MANAGER_H

#include <vector>
#include "components/SkullMinion.h"
#include "physics/PBD.h"
#include <memory>
#include "generation/Room.h"
#include "SteeringBehaviors.h"
#include "components/HealthComponent.h"

class SkullMinionManager
{
public:
    static SkullMinionManager* i_;
private:
    SkullMinionManager();
    ~SkullMinionManager() = default;

public:
    std::vector<std::shared_ptr<components::SkullMinion>> minions_;
    generation::Room* room_;

    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new SkullMinionManager();
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

    std::shared_ptr<components::SkullMinion> CreateSkullMinion(std::shared_ptr<GameObject> game_object);
    void AddMinion(std::shared_ptr<components::SkullMinion> minion);
    void RemoveMinion(std::shared_ptr<components::SkullMinion> minion);

    void UpdateMinion(std::shared_ptr<components::SkullMinion> minion);
    //void UpdateMinions();
};

#endif // !SKULL_MINION_MANAGER_H
