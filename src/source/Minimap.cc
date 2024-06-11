#include "../headers/Minimap.h"

Minimap::Minimap(s_ptr<GameObject> root)
{
    shader_ = res::get_shader("res/shaders/HUD.vert", "res/shaders/HUD.frag");
    texture_ = res::get_texture("res/textures/color.png");

    finished_texture_ = res::get_texture("res/textures/finished_room.png");
    unfinished_texture_ = res::get_texture("res/textures/unfinished_room.png");
    discovered_texture_ = res::get_texture("res/textures/discovered_room.png");
    current_texture_ = res::get_texture("res/textures/active_room.png");

    this->root_ = root;
}

void Minimap::Update(generation::RoomLayoutGenerator &rlg, generation::Room *current_room)
{
    for (auto &[pos, room] : rlg.rooms)
    {
        if (room.is_discovered || room.is_generated)
        {
            if (!minimap_rooms.contains(pos))
            {
                Rebuild(rlg);
                break;
            }
        }
    }
    for (auto &[pos, room] : minimap_rooms)
    {
        auto hud_renderer = room->GetComponent<components::HUDRenderer>();
        ColorRoom(rlg.rooms[pos], *current_room, hud_renderer);
    }
}

void Minimap::Rebuild(generation::RoomLayoutGenerator &rlg)
{
    for (auto &[pos, room] : minimap_rooms)
    {
        room->Destroy();
    }
    minimap_rooms.clear();
    glm::ivec2 bottom_left = glm::ivec2(9999999, 9999999);
    glm::ivec2 top_right = glm::ivec2(-9999999, -9999999);

    current_room_object = GameObject::Create();
    current_room_object->AddComponent(make_shared<components::HUDRenderer>(current_texture_, shader_));
    current_room_object->transform_->set_scale({1.1f, 1.1f, 1.0f});

    for (auto &[pos, room] : rlg.rooms)
    {
        if (room.is_discovered || room.is_generated)
        {
            auto room_object = GameObject::Create(root_);
            auto hud_renderer = make_shared<components::HUDRenderer>(texture_, shader_);

            room_object->AddComponent(hud_renderer);
            minimap_rooms[pos] = room_object;

            bottom_left.x = glm::min(bottom_left.x, pos.x);
            bottom_left.y = glm::min(bottom_left.y, pos.y);
            top_right.x = glm::max(top_right.x, pos.x);
            top_right.y = glm::max(top_right.y, pos.y);
        }
    }
    int width = top_right.x - bottom_left.x + 1;
    int height = top_right.y - bottom_left.y + 1;
    float segment_x_offset = (1.0f / width);
    float segment_y_offset = (1.0f / height);
    float segment_offset = glm::min(segment_x_offset, segment_y_offset);
    float x_offset = (bottom_left.x + top_right.x) * 0.5f;
    float y_offset = (bottom_left.y + top_right.y) * 0.5f;

    for (auto &[pos, room] : minimap_rooms)
    {
        float x = (-(pos.x - x_offset)) * segment_offset * 2.0f;
        float y = (-(pos.y - y_offset)) * segment_offset * 2.0f;
        room->transform_->set_position(glm::vec3(x, y, 0.0f));
        segment_scale = glm::min(segment_x_offset, segment_y_offset);
        room->transform_->set_scale(glm::vec3(segment_scale, segment_scale, 1.0f));
        // room->transform_->set_scale(glm::vec3(0.2f, 1.0f, 1.0f));
    }
}

void Minimap::ColorRoom(generation::Room &room, generation::Room &current_room, s_ptr<components::HUDRenderer> hud_renderer)
{
    if (room.is_generated)
    {
        if (room.enemies->transform_->children_.size() > 0)
        {
            hud_renderer->texture_ = unfinished_texture_;
        }
        else
        {
            hud_renderer->texture_ = finished_texture_;
        }
    }
    else
    {
        hud_renderer->texture_ = discovered_texture_;
    }

    if (room.position == current_room.position)
    {
        hud_renderer->gameObject_.lock()->transform_->set_scale({segment_scale, segment_scale, 1.0f});

        if (current_room_object->transform_->parent_)
        {
            current_room_object->transform_->parent_->RemoveChild(current_room_object->transform_);
        }
        hud_renderer->gameObject_.lock()->transform_->AddChild(current_room_object->transform_);
    }
    else
    {
        hud_renderer->gameObject_.lock()->transform_->set_scale({segment_scale * 0.75f, segment_scale * 0.75f, 1.0f});
    }
}
