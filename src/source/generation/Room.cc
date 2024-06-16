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
    barells = GameObject::Create(room_object);

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

void generation::Room::Destroy()
{
    room_object->Destroy();
    room_object = nullptr;
    enemies = nullptr;
    clutter = nullptr;
    lamps = nullptr;
    walls = nullptr;
    gates = nullptr;
    floors = nullptr;
    drops = nullptr;
    barells = nullptr;
}

generation::Room::Room(json j, s_ptr<GameObject> root)
{
    room_object = GameObject::Create(root);
    enemies = GameObject::Deserialize(j["enemies_root"]);
    clutter = GameObject::Deserialize(j["clutter_root"]);
    lamps = GameObject::Deserialize(j["lamps_root"]);
    walls = GameObject::Deserialize(j["walls_root"]);
    gates = GameObject::Deserialize(j["gates_root"]);
    floors = GameObject::Deserialize(j["floors_root"]);
    drops = GameObject::Deserialize(j["drops_root"]);
    barells = GameObject::Deserialize(j["barells_root"]);

    room_object->transform_->AddChild(enemies->transform_);
    room_object->transform_->AddChild(clutter->transform_);
    room_object->transform_->AddChild(lamps->transform_);
    room_object->transform_->AddChild(walls->transform_);
    room_object->transform_->AddChild(gates->transform_);
    room_object->transform_->AddChild(floors->transform_);
    room_object->transform_->AddChild(drops->transform_);
    room_object->transform_->AddChild(barells->transform_);

    position = {j["position"][0], j["position"][1]};

    up_gate = j["up_gate"];
    right_gate = j["right_gate"];
    down_gate = j["down_gate"];
    left_gate = j["left_gate"];

    is_generated = j["is_generated"];
    is_built = j["is_built"];
    is_discovered = j["is_discovered"];

    width = j["width"];
    height = j["height"];

    for (auto &i : j["up_walls_idx"])
    {
        up_walls_idx.push_back(i);
    }
    for (auto &i : j["left_walls_idx"])
    {
        left_walls_idx.push_back(i);
    }

    up_gate_idx = j["up_gate_idx"];
    right_gate_idx = j["right_gate_idx"];
    down_gate_idx = j["down_gate_idx"];
    left_gate_idx = j["left_gate_idx"];

    up_gate_wall = j["up_gate_wall"];
    right_gate_wall = j["right_gate_wall"];
    down_gate_wall = j["down_gate_wall"];
    left_gate_wall = j["left_gate_wall"];

    up_gate_pos = {j["up_gate_pos"][0], j["up_gate_pos"][1], j["up_gate_pos"][2]};
    right_gate_pos = {j["right_gate_pos"][0], j["right_gate_pos"][1], j["right_gate_pos"][2]};
    down_gate_pos = {j["down_gate_pos"][0], j["down_gate_pos"][1], j["down_gate_pos"][2]};
    left_gate_pos = {j["left_gate_pos"][0], j["left_gate_pos"][1], j["left_gate_pos"][2]};

    for (auto &i : j["lamp_positions"])
    {
        lamp_positions.push_back({i[0], i[1], i[2]});
    }
    for (auto &i : j["lamp_activity"])
    {
        lamp_activity.push_back(i);
    }

    for (auto &i : j["clutter_positions"])
    {
        clutter_positions.push_back({i[0], i[1], i[2]});
    }
    for (auto &i : j["clutter_idx"])
    {
        clutter_idx.push_back(i);
    }


    for (auto& i : j["barells_positions"])
    {
        barells_positions.push_back({ i[0], i[1], i[2] });
    }
    for (auto& i : j["barell_idx"])
    {
        barell_idx.push_back(i);
    }

    for (auto &i : j["enemies_positions"])
    {
        enemies_positions.push_back({i[0], i[1], i[2]});
    }
    for (auto &i : j["enemies_idx"])
    {
        enemies_idx.push_back(i);
    }
}

json generation::Room::Serialize()
{
    json j;

    j["enemies_root"] = enemies->Serialize();
    j["clutter_root"] = clutter->Serialize();
    j["lamps_root"] = lamps->Serialize();
    j["walls_root"] = walls->Serialize();
    j["gates_root"] = gates->Serialize();
    j["floors_root"] = floors->Serialize();
    j["drops_root"] = drops->Serialize();
    j["barells_root"] = barells->Serialize();

    j["position"] = {position.x, position.y};
    j["up_gate"] = up_gate;
    j["right_gate"] = right_gate;
    j["down_gate"] = down_gate;
    j["left_gate"] = left_gate;

    j["is_generated"] = is_generated;
    j["is_built"] = is_built;
    j["is_discovered"] = is_discovered;

    j["width"] = width;
    j["height"] = height;

    for (auto &i : up_walls_idx)
    {
        j["up_walls_idx"].push_back(i);
    }
    for (auto &i : left_walls_idx)
    {
        j["left_walls_idx"].push_back(i);
    }

    j["up_gate_idx"] = up_gate_idx;
    j["right_gate_idx"] = right_gate_idx;
    j["down_gate_idx"] = down_gate_idx;
    j["left_gate_idx"] = left_gate_idx;

    j["up_gate_wall"] = up_gate_wall;
    j["right_gate_wall"] = right_gate_wall;
    j["down_gate_wall"] = down_gate_wall;
    j["left_gate_wall"] = left_gate_wall;

    j["up_gate_pos"] = {up_gate_pos.x, up_gate_pos.y, up_gate_pos.z};
    j["right_gate_pos"] = {right_gate_pos.x, right_gate_pos.y, right_gate_pos.z};
    j["down_gate_pos"] = {down_gate_pos.x, down_gate_pos.y, down_gate_pos.z};
    j["left_gate_pos"] = {left_gate_pos.x, left_gate_pos.y, left_gate_pos.z};

    for (auto &i : lamp_positions)
    {
        j["lamp_positions"].push_back({i.x, i.y, i.z});
    }
    for (bool i : lamp_activity)
    {
        j["lamp_activity"].push_back(i);
    }

    for (auto &i : clutter_positions)
    {
        j["clutter_positions"].push_back({i.x, i.y, i.z});
    }
    for (auto &i : clutter_idx)
    {
        j["clutter_idx"].push_back(i);
    }

    for (auto& i : barells_positions)
    {
        j["barells_positions"].push_back({ i.x, i.y, i.z });
    }
    for (auto& i : barell_idx)
    {
        j["barell_idx"].push_back(i);
    }

    for (auto &i : enemies_positions)
    {
        j["enemies_positions"].push_back({i.x, i.y, i.z});
    }
    for (auto &i : enemies_idx)
    {
        j["enemies_idx"].push_back(i);
    }

    return j;
}
