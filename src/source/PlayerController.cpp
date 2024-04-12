#include "../headers/PlayerController.h"

Components::PlayerController::PlayerController(int gamepadID)
{
    this->gamepadID_ = gamepadID;
}

void Components::PlayerController::Start()
{
    transform_ = gameObject_.lock()->transform_;
    Input::InputManager::i_->AddObserver(gamepadID_, shared_from_this());

    move_generator_ = std::make_shared<physics::BasicGenerator>();
    pull_generator_ = std::make_shared<physics::BasicGenerator>();

    auto particle_component = gameObject_.lock()->GetComponent<Components::Particle>();

    physics::PhysicsManager::i_->AddFGRRecord(move_generator_, particle_component);
    physics::PhysicsManager::i_->AddFGRRecord(pull_generator_, particle_component);
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
            move_generator_->direction_ = glm::vec3(state.axis.x, 0.0f, state.axis.y);
            move_generator_->magnitude_ = speed_;
            break;
        }
        case Action::PULL_ROPE:
        {
            if (state.button)
            {
                pull_generator_->direction_ = move_generator_->direction_;
                pull_generator_->magnitude_ = pull_power_;
            }
            else
            {
                pull_generator_->direction_ = glm::vec3(0.0f);
                pull_generator_->magnitude_ = 0.0f;
            }
            break;
        }
    }
}
