#include "../headers/PlayerController.h"

Components::PlayerController::PlayerController(int gamepadID)
{
    this->gamepadID_ = gamepadID;
}

void Components::PlayerController::Start()
{
    transform_ = gameObject_.lock()->transform_;
    Input::InputManager::i_->AddObserver(gamepadID_, shared_from_this());

    move_generator_ = std::make_shared<pbd::BasicGenerator>();
    pull_generator_ = std::make_shared<pbd::BasicGenerator>();

    auto particle_component = gameObject_.lock()->GetComponent<Components::PBDParticle>();

    pbd::PBDManager::i_->CreateFGRRecord(particle_component, move_generator_);
    pbd::PBDManager::i_->CreateFGRRecord(particle_component, pull_generator_);
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
            if (state.button && !is_pulling_)
            {
                is_pulling_ = true;
                pull_generator_->direction_ = move_generator_->direction_;
                pull_generator_->magnitude_ = pull_power_;
                Timer::AddTimer(0.25f, [this]()
                {
                    pull_generator_->magnitude_ = 0;
                    std::cout << "Pulling stopped" << std::endl;
                });
                Timer::AddTimer(1.0f, [this](){ is_pulling_ = false; });
            }
            break;
        }
    }
}
