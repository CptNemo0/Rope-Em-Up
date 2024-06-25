#include "../../headers/components/Altar.h"

#include "../../headers/PlayerStatsManager.h"

void components::Altar::Start()
{
    player_1 = PlayerStatsManager::i_->player_1_;
    player_2 = PlayerStatsManager::i_->player_2_;
    transform_ = gameObject_.lock()->transform_;
    billboard_renderer_ = gameObject_.lock()->GetComponent<components::BillboardRenderer>();

    input::InputManager::i_->AddObserver(0, shared_from_this());
    input::InputManager::i_->AddObserver(1, shared_from_this());
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
                SceneManager::i_->current_scene_->SwitchMenu("altar");
            }
            break;
        }
    }
}
