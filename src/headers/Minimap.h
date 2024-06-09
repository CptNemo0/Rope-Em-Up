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
    void Rebuild(generation::RoomLayoutGenerator& rlg);
    void ColorRoom(generation::Room &room, s_ptr<components::HUDRenderer> hud_renderer);
public:
    s_ptr<Shader> shader_;
    s_ptr<tmp::Texture> texture_;
    s_ptr<GameObject> root_;
    Minimap(s_ptr<GameObject> root);

    std::unordered_map<glm::ivec2, s_ptr<GameObject>> minimap_rooms;

    void Update(generation::RoomLayoutGenerator& rlg, generation::Room *current_room);
};

#endif // MINIMAP_H