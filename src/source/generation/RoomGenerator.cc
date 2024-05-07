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
        room.up_gate = rooms.contains(position + glm::ivec2(0, 1));
        room.right_gate = rooms.contains(position + glm::ivec2(1, 0));
        room.down_gate = rooms.contains(position + glm::ivec2(0, -1));
        room.left_gate = rooms.contains(position + glm::ivec2(0, 1));
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
    settings_.healing_spots += 1;
}

void generation::RoomGenerator::DecreaseHealingSpots()
{
    settings_.healing_spots -= 1;
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

void generation::GenerateRoom(RoomGenerationSettings* rgs, RoomModels* rm, std::deque<w_ptr<GameObject>>& room_parts, s_ptr<GameObject> scene_root, s_ptr<Shader> shader)
{
    //generate upper walls

    for (int i = 0; i < rgs->width; i++)
    {
        int model_idx =  random::RandInt(0, rm->walls.size() - 1);
        s_ptr<GameObject> wall_up = GameObject::Create(scene_root);
        wall_up->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, 0.0f, 0.0f));
        wall_up->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
        wall_up->AddComponent(make_shared<components::MeshRenderer>(rm->walls[model_idx], shader));
        room_parts.push_back(wall_up);
        wall_up->PropagateStart();
    }

    //generate left walls

    for (int i = 0; i < rgs->height; i++)
    {
        int model_idx = random::RandInt(0, rm->walls.size() - 1);
        s_ptr<GameObject> wall_left = GameObject::Create(scene_root);
        wall_left->transform_->set_position(glm::vec3(0.0, 0.0f, -8.0f - i * kModuleSize));
        wall_left->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
        wall_left->AddComponent(make_shared<components::MeshRenderer>(rm->walls[model_idx], shader));
        room_parts.push_back(wall_left);
        wall_left->PropagateStart();
    }

    //generate floors
    int model_idx = random::RandInt(0, rm->floors.size() - 1);
    for (int i = 0; i < rgs->width; i++)
    {
        for (int j = 0; j < rgs->height; j++)
        {
            s_ptr<GameObject> floor = GameObject::Create(scene_root);
            floor->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize));
            floor->AddComponent(make_shared<components::MeshRenderer>(rm->floors[model_idx], shader));
            room_parts.push_back(floor);
            floor->PropagateStart();
        }
    }
}
