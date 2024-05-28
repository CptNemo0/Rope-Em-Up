#include "../../headers/generation/Room.h"

generation::Room::Room(glm::ivec2 position)
{
    this->position = position;
    room_object = nullptr;
    up_gate = false;
    right_gate = false;
    down_gate = false;
    left_gate = false;

    width = 2;
    height = 2;

    up_gate_idx = -1;
    right_gate_idx = -1;
    down_gate_idx = -1;
    left_gate_idx = -1;

    up_gate_wall = -1;
    right_gate_wall = -1;
    down_gate_wall = -1;
    left_gate_wall = -1;

    up_gate_pos = glm::vec3(0.0f);
    right_gate_pos = glm::vec3(0.0f);
    down_gate_pos = glm::vec3(0.0f);
    left_gate_pos = glm::vec3(0.0f);
}

generation::Room::Room(glm::ivec2 position, std::shared_ptr<GameObject> root)
{
    this->position = position;
    room_object = GameObject::Create(root);
    enemies = GameObject::Create(room_object);
    lamps = GameObject::Create(room_object);
    clutter = GameObject::Create(room_object);
    gates = GameObject::Create(room_object);
    walls = GameObject::Create(room_object);
    floors = GameObject::Create(room_object);
    drops = GameObject::Create(room_object);

    up_gate = false;
    right_gate = false;
    down_gate = false;
    left_gate = false;

    width = 2;
    height = 2;

    up_gate_idx = -1;
    right_gate_idx = -1;
    down_gate_idx = -1;
    left_gate_idx = -1;

    up_gate_wall = -1;
    right_gate_wall = -1;
    down_gate_wall = -1;
    left_gate_wall = -1;

    up_gate_pos = glm::vec3(0.0f);
    right_gate_pos = glm::vec3(0.0f);
    down_gate_pos = glm::vec3(0.0f);
    left_gate_pos = glm::vec3(0.0f);
}
