#include "../../headers/generation/RoomGenerator.h"

std::pair<int, glm::ivec2> generation::RoomLayoutGenerator::FindNextClosestPoint(const glm::ivec2 &A, const glm::ivec2 &B, const glm::ivec2 &point, int prev_direction)
{
    glm::ivec2 final_point;
    int final_direction;
    float min_distance = std::numeric_limits<float>::max();
    for (int i = 0; i < 4; i++)
    {
        if (prev_direction != -1
        &&  i == reverse_direction[prev_direction])
        {
            continue;
        }

        auto next_point = point + direction[i];

        if (next_point.x > glm::max(A.x, B.x)
        ||  next_point.y > glm::max(A.y, B.y)
        ||  next_point.x < glm::min(A.x, B.x)
        ||  next_point.y < glm::min(A.y, B.y))
        {
            continue;
        }

        float BxAx = static_cast<float>(B.x - A.x);
        float ByAy = static_cast<float>(B.y - A.y);
        float numerator = glm::abs(BxAx * (next_point.y - A.y) - (next_point.x - A.x) * ByAy);
        float denominator = glm::sqrt(BxAx * BxAx + ByAy * ByAy);
        float distance = numerator / denominator;
        if (distance < min_distance)
        {
            final_point = next_point;
            final_direction = i;
            min_distance = distance;
        }
        if (distance == min_distance)
        {
            auto next_end_distance = glm::distance(glm::vec2(next_point), glm::vec2(B));
            auto final_end_distance = glm::distance(glm::vec2(final_point), glm::vec2(B));
            if (next_end_distance < final_end_distance)
            {
                final_point = next_point;
                final_direction = i;
                min_distance = distance;
            }
        }
    }
    return {final_direction, final_point};
}

void generation::RoomLayoutGenerator::GenerateRoomsBetweenPoints(const glm::ivec2 &A, const glm::ivec2 &B, const RoomLayoutGenerationSettings &settings)
{

    glm::ivec2 current_point = A;
    int prev_direction = -1;

    AddRoom(current_point);
    int cap = 1000;
    do
    {
        auto result = FindNextClosestPoint(A, B, current_point, prev_direction);
        current_point = result.second;
        prev_direction = result.first;
        AddRoom(current_point);
        // cout << "start point: " << start_point.x << ", " << start_point.y << '\n';
        // cout << "end point: " << end_point.x << ", " << end_point.y << '\n';
        // cout << "current point: " << current_point.x << ", " << current_point.y << "\n\n";
        if (--cap <= 0)
        {
            cout << "Something broke...\n";
            break;
        }
    }
    while (current_point != B);
}

void generation::RoomLayoutGenerator::GenerateRooms(const RoomLayoutGenerationSettings &settings)
{
    rooms.clear();
    rooms_ordered.clear();

    auto start_point = glm::ivec2(0, 0);
    for (int i = 0; i < settings.branch_division_count; i++)
    {
        glm::vec2 random_direction = random::RandOnArc(settings.angle, settings.span);
        random_direction *= random::RandFloat(settings.branch_division_min_length, settings.branch_division_max_length);
        glm::ivec2 end_point = start_point + glm::ivec2(glm::round(random_direction));

        GenerateRoomsBetweenPoints(start_point, end_point, settings);

        start_point = end_point;
    }

    int branch_index_divisor = settings.sub_branch_count + 1;
    float branch_index_distance = static_cast<float>(rooms_ordered.size()) / branch_index_divisor;

    std::deque<Room> branching_rooms;

    for (int i = 1; i <= settings.sub_branch_count; i++)
    {
        int index = glm::floor(i * branch_index_distance);
        auto room = rooms_ordered[index];
        branching_rooms.push_back(room);
    }

    std::vector<float> sub_branch_directions;
    sub_branch_directions.resize(settings.sub_branch_count);
    int middle = settings.sub_branch_count / 2;
    for (int i = 0; i < settings.sub_branch_count; i++)
    {
        if (i < middle)
        {
            sub_branch_directions[i] = 0.25f;
        }
        else
        {
            sub_branch_directions[i] = -0.25f;
        }
    }

    std::shuffle(sub_branch_directions.begin(), sub_branch_directions.end(), random::get_random_device());

    for (int i = 0; i < settings.sub_branch_count; i++)
    {
        auto room = branching_rooms[i];
        auto sub_branch_direction = sub_branch_directions[i];

        glm::vec2 random_direction = random::RandOnArc(settings.angle + sub_branch_direction, settings.sub_branch_span);
        random_direction *= random::RandFloat(settings.sub_branch_min_length, settings.sub_branch_max_length);
        glm::ivec2 end_point = room.position + glm::ivec2(glm::round(random_direction));

        GenerateRoomsBetweenPoints(room.position, end_point, settings);
    }
}

void generation::RoomLayoutGenerator::GenerateGates()
{
    for (auto& [position, room] : rooms)
    {
        room.up_gate = rooms.contains(position + glm::ivec2(0, -1));
        room.right_gate = rooms.contains(position + glm::ivec2(-1, 0));
        room.down_gate = rooms.contains(position + glm::ivec2(0, 1));
        room.left_gate = rooms.contains(position + glm::ivec2(1, 0));
    }
}

void generation::RoomLayoutGenerator::AddRoom(glm::ivec2 position)
{
    int rooms_size_before = rooms.size();
    rooms.insert(std::make_pair(position, Room(position)));
    int rooms_size_after = rooms.size();
    if (rooms_size_before != rooms_size_after)
    {
        rooms_ordered.push_back(Room(position));
    }
}

void generation::RoomGenerator::IncreaseSize()
{
    int r = random::RandInt(0, 1);

    if (r)
    {
        settings_.height += 1;
    }
    else
    {
        settings_.width += 1;
    }
}

void generation::RoomGenerator::DecreaseSize()
{
    if (settings_.width == 1 && settings_.height == 1)
    {

    }
    else if (settings_.width == 1)
    {
        settings_.height -= 1;
    }
    else if (settings_.height == 1)
    {
        settings_.width -= 1;
    }
    else
    {
        int r = random::RandInt(0, 1);

        if (r)
        {
            settings_.height -= 1;
        }
        else
        {
            settings_.width -= 1;
        }
    }
}

void generation::RoomGenerator::IncreaseEnemies()
{
    settings_.enemies += 1;
}

void generation::RoomGenerator::DecreaseEnemies()
{
    settings_.enemies -= 1;
}

void generation::RoomGenerator::IncreaseHealingSpots()
{
    settings_.lamps += 1;
}

void generation::RoomGenerator::DecreaseHealingSpots()
{
    settings_.lamps -= 1;
}

void generation::RoomGenerator::GenerateGrid(Room& room)
{
    for (float i = kOffset; i < room.width - kOffset; i += kOffset)
    {
        for (float j = kOffset; j < room.height - kOffset; j += kOffset)
        {
            room.grid.insert(glm::ivec2(i, j));
        }
    }
}

void generation::RoomGenerator::GenerateEnemies(Room& room)
{
}

void generation::RoomGenerator::GenerateHealingSpots(Room& room)
{
}

void generation::RoomGenerator::Generate()
{

}

void generation::GenerateRoom(Room& room, RoomGenerationSettings* rgs, RoomModels* rm)
{
    //generate upper walls
    room.width = rgs->width;
    room.height = rgs->height;

    room.up_walls_idx.reserve(rgs->width);
    for (int i = 0; i < rgs->width; i++)
    {
        int model_idx =  random::RandInt(0, rm->walls.size() - 1);
        room.up_walls_idx.push_back(model_idx);
    }

    //generate left walls
    room.left_walls_idx.reserve(rgs->height);
    for (int i = 0; i < rgs->height; i++)
    {
        int model_idx = random::RandInt(0, rm->walls.size() - 1);
        room.left_walls_idx.push_back(model_idx);
    }

    //generate gates
    //up

    int model_idx = 0;
    if (room.up_gate)
    {
        room.up_gate_idx = random::RandInt(0, rm->gates.size() - 1);
        room.up_gate_wall = random::RandInt(0, room.width - 1);
        room.up_gate_pos = glm::vec3(-8.0f - room.up_gate_wall * kModuleSize, 0.0f, 0.0f);
    }
    
    //down
    if (room.down_gate)
    {
        room.down_gate_idx = random::RandInt(0, rm->gates.size() - 1);
        room.down_gate_wall = random::RandInt(0, room.width - 1);
        room.down_gate_pos = glm::vec3(-8.0f - room.down_gate_wall * kModuleSize, 0.0f, -room.height * kModuleSize);
    }
    
    //right
    if (room.right_gate)
    {
        room.right_gate_idx = random::RandInt(0, rm->gates.size() - 1);
        room.right_gate_wall = random::RandInt(0, room.height - 1);
        room.right_gate_pos = glm::vec3(-room.width * kModuleSize, 0.0f, -8.0f - room.right_gate_wall * kModuleSize);
    }

    //left
    if (room.left_gate)
    {
        room.left_gate_idx = random::RandInt(0, rm->gates.size() - 1);
        room.left_gate_wall = random::RandInt(0, room.height - 1);
        room.left_gate_pos = glm::vec3(0.0f, 0.0f, -8.0 - room.left_gate_wall * kModuleSize);
    }

    room.is_generated = true;

    // Lamps
     
    int lamp_num = rgs->lamps;
    std::unordered_set<glm::vec3> lamp_set;

    for (int i = 0; i < room.width; i++)
    {
        for (int j = 0; j < room.height; j++)
        {
            auto pos = glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize);
            for (int k = 0; k < kLanternPlacement.size(); k++)
            {
                lamp_set.insert(kLanternPlacement[k] + pos);
            }
        }
    }

    // Left top
    if (lamp_num > 0)
    {
        auto left_top = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - 0 * kModuleSize);
        left_top += kLanternPlacement[0];
        room.lamp_positions.push_back(left_top);
        lamp_set.erase(left_top);
        lamp_num--;
        
    }
    
    // Right bot
    if (lamp_num > 0)
    {
        auto right_bot = glm::vec3(-8.0f - (room.width - 1) * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);
        right_bot += kLanternPlacement[3];
        room.lamp_positions.push_back(right_bot);
        lamp_set.erase(right_bot);
        lamp_num--;
    }
    
    // Right top
    if (lamp_num > 0)
    {
        auto right_top = glm::vec3(-8.0f - (room.width - 1) * kModuleSize, 0.0f, -8.0f - 0 * kModuleSize);
        right_top += kLanternPlacement[1];
        room.lamp_positions.push_back(right_top);
        lamp_set.erase(right_top);
        lamp_num--;
    }
    
    if (lamp_num > 0)
    {
        // Left bot
        auto left_bot = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);
        left_bot += kLanternPlacement[2];
        room.lamp_positions.push_back(left_bot);
        lamp_set.erase(left_bot);
        lamp_num--;
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::vector<glm::vec3> shuffled_set = std::vector<glm::vec3>(lamp_set.begin(), lamp_set.end());

    std::shuffle(shuffled_set.begin(), shuffled_set.end(), g);
    
    if (lamp_num > 0)
    {
        for (auto& pos : shuffled_set)
        {
            room.lamp_positions.push_back(pos);
            lamp_num--;
            if (lamp_num == 0)
            {
                break;
            }
        }
    }
    }
    

void generation::BuildRoom(const Room& room, RoomModels* rm, std::deque<w_ptr<GameObject>>& room_parts, s_ptr<GameObject> scene_root, s_ptr<Shader> shader)
{
    for (int i = 0; i < room.width; i++)
    {
        s_ptr<GameObject> wall_up = GameObject::Create(scene_root);
        wall_up->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, 0.0f, 0.0f));
        wall_up->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
        wall_up->AddComponent(make_shared<components::MeshRenderer>(rm->walls[room.up_walls_idx[i]], shader));
        room_parts.push_back(wall_up);
        wall_up->PropagateStart();
    }

    for (int i = 0; i < room.height; i++)
    {
        s_ptr<GameObject> wall_left = GameObject::Create(scene_root);
        wall_left->transform_->set_position(glm::vec3(0.0, 0.0f, -8.0f - i * kModuleSize));
        wall_left->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
        wall_left->AddComponent(make_shared<components::MeshRenderer>(rm->walls[room.left_walls_idx[i]], shader));
        room_parts.push_back(wall_left);
        wall_left->PropagateStart();
    }

    for (int i = 0; i < room.width; i++)
    {
        for (int j = 0; j < room.height; j++)
        {
            s_ptr<GameObject> floor = GameObject::Create(scene_root);
            floor->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize));
            floor->AddComponent(make_shared<components::MeshRenderer>(rm->floors[0], shader));
            room_parts.push_back(floor);
            floor->PropagateStart();
        }
    }

    //generate gates
    //up
    if (room.up_gate)
    {
        s_ptr<GameObject> gate = GameObject::Create(scene_root);
        gate->transform_->set_position(room.up_gate_pos);
        gate->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
        gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
        room_parts.push_back(gate);
        gate->PropagateStart();

    }

    //down
    if (room.down_gate)
    {
        s_ptr<GameObject> gate = GameObject::Create(scene_root);
        gate->transform_->set_position(room.down_gate_pos);
        gate->transform_->set_rotation(glm::vec3(0.0f, 0.0, 0.0f));
        gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
        room_parts.push_back(gate);
        gate->PropagateStart();
    }

    //right
    if (room.right_gate)
    {
        s_ptr<GameObject> gate = GameObject::Create(scene_root);
        gate->transform_->set_position(room.right_gate_pos);
        gate->transform_->set_rotation(glm::vec3(0.0f, 90.0f, 0.0f));
        gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
        room_parts.push_back(gate);
        gate->PropagateStart();
    }

    //left
    if (room.left_gate)
    {
        s_ptr<GameObject> gate = GameObject::Create(scene_root);
        gate->transform_->set_position(room.left_gate_pos);
        gate->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
        gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
        room_parts.push_back(gate);
        gate->PropagateStart();
    }

    // generate lamps

    for (auto pos : room.lamp_positions)
    {
        s_ptr<GameObject> lamp = GameObject::Create(scene_root);
        lamp->transform_->set_position(pos);
        lamp->AddComponent(make_shared<components::MeshRenderer>(rm->lamps[0], shader));
        lamp->AddComponent(collisions::CollisionManager::i_->CreateCollider(0, gPRECISION, rm->lamps[0]->meshes_[0], lamp->transform_));
        room_parts.push_back(lamp);
        lamp->PropagateStart();
    }
}