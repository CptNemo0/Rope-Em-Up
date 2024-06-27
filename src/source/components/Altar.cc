#include "../../headers/components/Altar.h"

#include "../../headers/PlayerStatsManager.h"
#include "../../headers/components/ParticleEmitter.h"

void components::Altar::Start()
{
    player_1 = PlayerStatsManager::i_->player_1_;
    player_2 = PlayerStatsManager::i_->player_2_;
    transform_ = gameObject_.lock()->transform_;
    billboard_renderer_ = gameObject_.lock()->GetComponent<components::BillboardRenderer>();

    input::InputManager::i_->AddObserver(0, shared_from_this());
    input::InputManager::i_->AddObserver(1, shared_from_this());

    auto emitter = gameObject_.lock()->transform_->parent_->children_[1]->game_object_->GetComponent<components::ParticleEmitter>();
    auto emitter2 = gameObject_.lock()->transform_->parent_->children_[2]->game_object_->GetComponent<components::ParticleEmitter>();

    timer_id_1_ = Timer::AddTimer(1.0f, nullptr, [emitter](float delta_time)
    {
        if (emitter->active_ && !emitter->dirty_)
        {
            static float t = 0.0f;
            t += delta_time;
            float x = 5.0f * glm::sin(t);
            float y = 5.0f * glm::cos(t);
            emitter->start_position_ = glm::vec3(x, 1.0f, y);
        }
    }, true);

    timer_id_2_ = Timer::AddTimer(1.0f, nullptr, [emitter2](float delta_time)
    {
        if (emitter2->active_ && !emitter2->dirty_)
        {
            static float t = 0.0f;
            t += delta_time;
            float x = -5.0f * glm::sin(t);
            float y = -5.0f * glm::cos(t);
            emitter2->start_position_ = glm::vec3(x, 1.0f, y);
        }
    }, true);
}

void components::Altar::Update()
{
    float distance_1 = glm::distance(player_1->transform_->get_global_position(), transform_->get_global_position());
    float distance_2 = glm::distance(player_2->transform_->get_global_position(), transform_->get_global_position());

    if (glm::max(distance_1, distance_2) <= 4.5f)
    {
        billboard_renderer_->active_ = true;
        is_near_ = true;
    }
    else
    {
        billboard_renderer_->active_ = false;
        is_near_ = false;
    }
}

void components::Altar::Destroy()
{
    input::InputManager::i_->RemoveObserver(0, shared_from_this());
    input::InputManager::i_->RemoveObserver(1, shared_from_this());
    Timer::RemoveTimer(timer_id_1_);
    Timer::RemoveTimer(timer_id_2_);
}

void components::Altar::OnAction(Action action, input::State state)
{
    if (!active_)
    {
        return;
    }
    switch (action)
    {
        case Action::CAST_SPELL:
        {
            if (!SceneManager::i_->current_scene_->menus_["altar"]->active_ && state.button)
            {
                player_1->GetComponent<components::PlayerController>()->active_ = false;
                player_2->GetComponent<components::PlayerController>()->active_ = false;
                Timer::AddTimer(0.1f, []()
                {
                    SceneManager::i_->current_scene_->SwitchMenu("altar");
                });
            }
            break;
        }
    }
}
