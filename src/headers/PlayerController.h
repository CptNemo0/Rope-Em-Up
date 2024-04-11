#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "GameObject.h"
#include "InputObserver.h"
#include "InputManager.h"

namespace Components
{

class PlayerController : public Component, public Input::InputObserver, public std::enable_shared_from_this<PlayerController>
{
private:
    int gamepadID_;
    std::shared_ptr<Transform> transform_;
public:
    PlayerController(int gamepadID);

    void Start() override;
    void Update() override;

    void OnAction(Action action, Input::State state) override;
};

} // namespace Components

#endif // !PLAYERCONTROLLER_H