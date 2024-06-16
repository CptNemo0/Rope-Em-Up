#ifndef DIFFICULTY_MANAGER_H
#define DIFFICULTY_MANAGER_H

#include "GameObject.h"
#include "generation/Room.h"
#include "generation/RoomGenerator.h"
#include "components/HealthComponent.h"

class DifficultyManager
{
public:
    static DifficultyManager* i_;
private:
    DifficultyManager();
    ~DifficultyManager() = default;

public:

    generation::Room* room_;
    float player_1_hp_;
    float player_2_hp_;
    float health_diff_;

    void UpdateRoom(generation::Room* room);
    void UpdateHealth(std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2);

    void UpdateSettings(generation::RoomGenerationSettings* rg_settings);

    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new DifficultyManager();
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

#endif // !DIFFICULTY_MANAGER_H