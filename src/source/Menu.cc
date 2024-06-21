#include "../headers/Menu.h"

void Menu::UpdateSelection()
{
    if (selection_outline_->transform_->parent_)
    {
        selection_outline_->transform_->parent_->scale({1.0f / 1.1f, 1.0f / 1.1f, 1.0f});
        selection_outline_->transform_->parent_->RemoveChild(selection_outline_->transform_);
    }
    layout_[current_pos_]->object_->transform_->AddChild(selection_outline_->transform_);
    selection_outline_->transform_->parent_->scale({1.1f, 1.1f, 1.0f});
}

void Menu::Disable()
{
    active_ = false;
    for (auto &item : layout_)
    {
        item.second->object_->Disable();
    }
}

void Menu::Enable()
{
    active_ = true;
    for (auto &item : layout_)
    {
        item.second->object_->Enable();
    }
}

Menu::Menu()
{
    auto selection_texture_ = res::get_texture("res/textures/menu_select.png");
    auto HUD_shader = res::get_shader("res/shaders/HUD.vert", "res/shaders/HUD.frag");

    selection_outline_ = GameObject::Create();
    selection_outline_->transform_->set_scale({0.95f, 0.9f, 1.0f});
    selection_outline_->AddComponent(make_shared<components::HUDRenderer>(selection_texture_, HUD_shader, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
}

void Menu::OnAction(Action action, input::State state)
{
    if (!active_)
    {
        return;
    }
    switch (action)
    {
        case Action::MENU_MOVE:
        {
            static float scale = glm::sqrt(2) * 0.5f;
            glm::ivec2 move_dir = {glm::round(state.axis.x), glm::round(state.axis.y)};
            move_dir *= -1;

            if (move_dir != glm::ivec2(0))
            {
                move_lock_ = true;
                glm::ivec2 next_pos = current_pos_;
                while (true)
                {
                    if (layout_.contains(next_pos + move_dir))
                    {
                        next_pos += move_dir;
                        if (!layout_[next_pos]->enabled_)
                            continue;
                        else
                        {
                            current_pos_ = next_pos;
                            break;
                        }
                    }
                    else
                        break;
                }
            }
            else
            {
                move_lock_ = false;
            }
            UpdateSelection();
            break;
        }
        case Action::MENU_CLICK:
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
