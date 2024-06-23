#ifndef CHOKE_INDICATOR_H
#define CHOKE_INDICATOR_H

#include <memory>

#include "res/Resources.h"
#include "Texture.h"
#include "SceneManager.h"
#include "components/PlayerController.h"
#include "GameObject.h"
#include "components/HUDRenderer.h"
#include "physics/Rope.h"

class ChokeIndicator
{
public:
    static ChokeIndicator* i_;
private:
    ChokeIndicator(std::shared_ptr<components::PlayerController> controller_1, std::shared_ptr<components::PlayerController> controller_2, Rope *rope);
    ~ChokeIndicator() = default;

public:

    std::shared_ptr<components::PlayerController> controller_1_;
    std::shared_ptr<components::PlayerController> controller_2_;
    Rope *rope_;
    
    std::shared_ptr<tmp::Texture> indicator_0_;
    std::shared_ptr<tmp::Texture> indicator_1_;
    std::shared_ptr<tmp::Texture> indicator_2_;
    std::shared_ptr<tmp::Texture> indicator_3_;

    std::shared_ptr<GameObject> renderer_;

    bool choking_;
    float timer_;

    void Update(float delta_time);

    static void Initialize(std::shared_ptr<components::PlayerController> controller_1, std::shared_ptr<components::PlayerController> controller_2, Rope *rope)
    {
        if (i_ == nullptr)
        {
            i_ = new ChokeIndicator(controller_1, controller_2, rope);
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

#endif // !CHOKE_INDICATOR_H
