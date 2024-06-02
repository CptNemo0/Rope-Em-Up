#include "../../headers/components/PlayerController.h"

components::PlayerController::PlayerController(int gamepadID)
{
    this->gamepadID_ = gamepadID;
}

void components::PlayerController::Start()
{
    input::InputManager::i_->AddObserver(gamepadID_, shared_from_this());

    move_generator_ = make_shared<pbd::BasicGenerator>();
    move_generator_->magnitude_ = speed_;

    pull_generator_ = make_shared<pbd::BasicGenerator>();

    auto particle_component = gameObject_.lock()->GetComponent<components::PBDParticle>();

    pbd::PBDManager::i_->CreateFGRRecord(particle_component, move_generator_);
    pbd::PBDManager::i_->CreateFGRRecord(particle_component, pull_generator_);
}

void components::PlayerController::Update()
{
    if (!is_pulling_)
    {
        move_generator_->direction_ = direction_;
    }
}

void components::PlayerController::Destroy()
{
    input::InputManager::i_->RemoveObserver(gamepadID_, shared_from_this());
    pbd::PBDManager::i_->RemoveRecord(move_generator_);
    pbd::PBDManager::i_->RemoveRecord(pull_generator_);
}

void components::PlayerController::OnAction(Action action, input::State state)
{
    if (!active_)
    {
        return;
    }
    switch (action)
    {
        case Action::MOVE:
        {
            direction_ = glm::vec3(state.axis.x, 0.0f, state.axis.y);
            break;
        }
        case Action::PULL_ROPE:
        {
            if (state.button && !pulling_cooldown_ && glm::length(move_generator_->direction_) >= 0.01f)
            {
                pulling_cooldown_ = true;
                Timer::AddTimer(1.0f, [this]()
                {
                    pulling_cooldown_ = false;
                });

                pull_generator_->direction_ = move_generator_->direction_;
                pull_generator_->magnitude_ = pull_power_;
                is_pulling_ = true;
                Timer::AddTimer(0.25f, [this]()
                {
                    pull_generator_->magnitude_ = 0;
                    is_pulling_ = false;
                });
            }
            break;
        }
    }
}

components::PlayerController::PlayerController(json &j)
{
    int gamepadID = j["gamepad_id"];
    this->PlayerController::PlayerController(gamepadID);
}

json components::PlayerController::Serialize()
{
    json j;

    j["gamepad_id"] = gamepadID_;

    return j;
}
