#include "../headers/Menu.h"

void Menu::UpdateSelection()
{
    if (selection_outline_->transform_->parent_)
    {
        selection_outline_->transform_->parent_->RemoveChild(selection_outline_->transform_);
    }
    layout_[current_pos_]->object_->transform_->AddChild(selection_outline_->transform_);
}

Menu::Menu()
{
    auto selection_texture_ = res::get_texture("res/textures/active_room.png");
    auto HUD_shader = res::get_shader("res/shaders/HUD.vert", "res/shaders/HUD.frag");

    selection_outline_ = GameObject::Create();
    selection_outline_->transform_->set_scale({1.1f, 1.1f, 1.0f});
    selection_outline_->AddComponent(make_shared<components::HUDRenderer>(selection_texture_, HUD_shader));
}

void Menu::OnAction(Action action, input::State state)
{
    if (!active_)
    {
        return;
    }
    switch (action)
    {
        case Action::MOVE:
        {
            static float scale = glm::sqrt(2) * 0.5f;
            glm::ivec2 move_dir = {glm::round(state.axis.x), glm::round(state.axis.y)};
            move_dir *= -1;

            if (move_dir != glm::ivec2(0))
            {
                move_lock_ = true;
                if (layout_.contains(current_pos_ + move_dir))
                {
                    current_pos_ += move_dir;
                }
            }
            else
            {
                move_lock_ = false;
            }
            UpdateSelection();
            break;
        }
        case Action::PULL_ROPE:
        {
            if (state.button)
            {
                if (layout_[current_pos_]->OnPress)
                {
                    layout_[current_pos_]->OnPress();
                }
            }
        }
    }
}
