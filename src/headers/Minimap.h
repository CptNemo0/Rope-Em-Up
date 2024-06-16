#ifndef MINIMAP_H
#define MINIMAP_H

#include <memory>
#include <unordered_map>

#include "GameObject.h"
#include "generation/RoomGenerator.h"
#include "components/HUDRenderer.h"
#include "res/Resources.h"
#include "typedef.h"

class Minimap
{
private:
    void ColorRoom(generation::Room &room, generation::Room &current_room, s_ptr<components::HUDRenderer> hud_renderer);

    s_ptr<tmp::Texture> finished_texture_;
    s_ptr<tmp::Texture> unfinished_texture_;
    s_ptr<tmp::Texture> discovered_texture_;
    s_ptr<tmp::Texture> current_texture_;

    float segment_scale = 1.0f;
public:
    s_ptr<Shader> shader_;
    s_ptr<tmp::Texture> texture_;
    s_ptr<GameObject> root_;
    Minimap(s_ptr<GameObject> root);

    std::unordered_map<glm::ivec2, s_ptr<GameObject>> minimap_rooms;
    s_ptr<GameObject> current_room_object;

    void Update(generation::RoomLayoutGenerator& rlg, generation::Room *current_room);
    void Rebuild(generation::RoomLayoutGenerator& rlg);
};

#endif // MINIMAP_H