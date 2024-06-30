#include "../../headers/components/PlayerController.h"
#include "../../headers/SpellCaster.h"
#include "../../headers/SceneManager.h"
#include "../../headers/drop/DropManager.h"

components::PlayerController::PlayerController(int gamepadID)
{
    this->gamepadID_ = gamepadID;
    std::vector<s_ptr<audio::AudioBuffer>> data;
    std::vector<s_ptr<audio::AudioBuffer>> damage_data;
    if (gamepadID == GLFW_JOYSTICK_1)
    {
        auto anim = res::get_animation("res/players/female/kobieta.fbx", 5, "res/players/female/kobieta.fbx");
        walk_timer_delay_ = anim->GetDuration() / anim->GetTicksPerSecond() * 0.5f;
        data.push_back(res::get_sound("res/sounds/walk3.wav"));
        data.push_back(res::get_sound("res/sounds/walk4.wav"));
        damage_data.push_back(res::get_sound("res/sounds/woman1.wav"));
        damage_data.push_back(res::get_sound("res/sounds/woman2.wav"));
        damage_data.push_back(res::get_sound("res/sounds/woman3.wav"));
    }
    if (gamepadID == GLFW_JOYSTICK_2)
    {
        auto anim = res::get_animation("res/players/male/facet.fbx", 5, "res/players/male/facet.fbx");
        walk_timer_delay_ = anim->GetDuration() / anim->GetTicksPerSecond() * 0.25f;
        data.push_back(res::get_sound("res/sounds/walk1.wav"));
        data.push_back(res::get_sound("res/sounds/walk2.wav"));
        damage_data.push_back(res::get_sound("res/sounds/man1.wav"));
        damage_data.push_back(res::get_sound("res/sounds/man2.wav"));
        damage_data.push_back(res::get_sound("res/sounds/man3.wav"));
    }
    walk_sounds_.SetData(data);
    damage_sounds_.SetData(damage_data);

    auto grass_data = std::vector<s_ptr<audio::AudioBuffer>>();
    grass_data.push_back(res::get_sound("res/sounds/grass1.wav"));
    grass_data.push_back(res::get_sound("res/sounds/grass2.wav"));
    grass_data.push_back(res::get_sound("res/sounds/grass3.wav"));
    grass_walk_sounds.SetData(grass_data);
}

void components::PlayerController::ForceStop()
{
    active_ = false;
    gameObject_.lock()->GetComponent<components::Animator>()->SetAnimation("Idle", 1);
    if (walking_timer_lock_)
    {
        Timer::RemoveTimer(walk_timer_);
        walking_timer_lock_ = false;
    }
    direction_ = glm::vec3(0.0f);
    move_generator_->direction_ = glm::vec3(0.0f);
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

    // Walking sounds
    if (gameObject_.lock()->GetComponent<components::Animator>()->IsAnimation("Run"))
    {
        if (!walking_timer_lock_)
        {
            if (SkullMinionManager::i_->room_->is_altar)
            {
                audio::AudioManager::i_->PlaySound(grass_walk_sounds.Pop(), 0.5f);
            }
            else
            {
                audio::AudioManager::i_->PlaySound(walk_sounds_.Pop(), 0.2f);
            }
            walk_timer_ = Timer::AddTimer(walk_timer_delay_, [this]()
            {
                if (SkullMinionManager::i_->room_->is_altar)
                {
                    audio::AudioManager::i_->PlaySound(grass_walk_sounds.Pop(), 0.5f);
                }
                else
                {
                    audio::AudioManager::i_->PlaySound(walk_sounds_.Pop(), 0.2f);
                }
            }, nullptr, true); 
            walking_timer_lock_ = true;
        }
    }
    else
    {
        if (walking_timer_lock_)
        {
            Timer::RemoveTimer(walk_timer_);
            walking_timer_lock_ = false;
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
            if (state.button)
            {
                SpellCaster::i_->QueueUpSpell(gameObject_.lock()->GetComponent<components::SpellSlotComponent>());
                break;
            }
        }
        case Action::PAUSE_GAME:
        {
            if (state.button)
            {
                SceneManager::i_->SwitchScene("pause_menu");
            }
            break;
        }
        case Action::INTERACT:
        {
            if (state.button)
            {
                drop::DropManager::i_->PickUpSpell(gameObject_.lock());
            }
            
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
