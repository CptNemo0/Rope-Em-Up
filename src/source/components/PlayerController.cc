#include "../../headers/components/PlayerController.h"
#include "../../headers/SpellCaster.h"
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
    move_generator_->direction_ = direction_;
    if (move_lock_)
    {
        move_generator_->magnitude_ = 0.0f;
    }
    else
    {
        move_generator_->magnitude_ = speed_;
        if (glm::length(direction_) > 0.0f)
        {
            gameObject_.lock()->GetComponent<components::Animator>()->SetAnimation("Run", 1);
        }
        else
        {
            gameObject_.lock()->GetComponent<components::Animator>()->SetAnimation("Idle", 1);
        }
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
            auto state_axis = glm::rotate(state.axis, glm::radians(Global::i_->active_camera_->yaw_ - 90.0f));
            direction_ = glm::vec3(state_axis.x, 0.0f, state_axis.y);
            if (glm::length(direction_) > 0.0f)
            {
                pull_direction_ = glm::normalize(direction_);
			}
            break;
        }
        case Action::PULL_ROPE:
        {
            if (glm::min(state.axis.x, state.axis.y) >= 0.95f)
            {
                if (!pull_lock_)
                {
                    is_pulling_ = true;
                }
            }
            if (glm::max(state.axis.x, state.axis.y) <= 0.05f)
            {
                pull_lock_ = false;
                is_pulling_ = false;
            }
            break;
        }
        case Action::CAST_SPELL:
        {
            cout << "AAAAAAAAAAAA" << endl;
            SpellCaster::i_->QueueUpSpell(gameObject_.lock()->GetComponent<components::SpellSlotComponent>());
            break;
        }
    }
}

void components::PlayerController::PullRope()
{
    is_pulling_ = false;
    move_lock_ = true;
    pull_lock_ = true;

    pull_generator_->direction_ = pull_direction_;
    pull_generator_->magnitude_ = pull_power_;

    Timer::AddTimer(0.5f, [this]()
    {
        pull_generator_->magnitude_ = 0;
        move_lock_ = false;
    });
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
