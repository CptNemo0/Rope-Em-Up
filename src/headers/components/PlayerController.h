#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "../global.h"
#include "../GameObject.h"
#include "../input/InputObserver.h"
#include "../input/InputManager.h"
#include "../physics/PBD.h"
#include "../Timer.h"

namespace components
{

class PlayerController : public Component, public input::InputObserver, public std::enable_shared_from_this<PlayerController>
{
public:
    int gamepadID_;
    int speed_ = 1000;
    int pull_power_ = 1000;

    s_ptr<pbd::BasicGenerator> move_generator_;
    s_ptr<pbd::BasicGenerator> pull_generator_;
    bool is_pulling_ = false, pulling_cooldown_ = false;
    glm::vec3 direction_ = glm::vec3(0.0f);
    PlayerController(int gamepadID);

    void Start() override;
    void Update() override;
    void Destroy() override;

    void OnAction(Action action, input::State state) override;
};

} // namespace Components

#endif // !PLAYERCONTROLLER_H