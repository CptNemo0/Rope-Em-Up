#include "../headers/PlayerController.h"

Components::PlayerController::PlayerController(int gamepadID)
{
    this->gamepadID_ = gamepadID;
}

void Components::PlayerController::Start()
{
    transform_ = gameObject_.lock()->transform_;
    Input::InputManager::i_->AddObserver(gamepadID_, shared_from_this());
}

void Components::PlayerController::Update()
{
}

void Components::PlayerController::OnAction(Action action, Input::State state)
{
    switch (action)
    {
        case Action::MOVE:
        {
            glm::vec2 axis = state.axis;
            std::cout << "Move: " << axis.x << ", " << axis.y << std::endl;
        }
    }
}
