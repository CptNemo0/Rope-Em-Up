#include "../../headers/generation/RoomGenerator.h"
#include <headers/animation/AnimatorManager.h>
#include "../../headers/components/Altar.h"

//std::deque<w_ptr<GameObject>> generation::Room::room_parts = std::deque<w_ptr<GameObject>>();
//std::vector<w_ptr<GameObject>> generation::Room::enemies = std::vector<w_ptr<GameObject>>();


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

void generation::RoomLayoutGenerator::GenerateRoomsBetweenPoints(const glm::ivec2 &A, const glm::ivec2 &B, const RoomLayoutGenerationSettings &settings, std::shared_ptr<GameObject> root)
{

    glm::ivec2 current_point = A;
    int prev_direction = -1;

    AddRoom(current_point, root);
    int cap = 1000;
    do
    {
        auto result = FindNextClosestPoint(A, B, current_point, prev_direction);
        current_point = result.second;
        prev_direction = result.first;
        AddRoom(current_point, root);
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

generation::RoomLayoutGenerator::RoomLayoutGenerator()
{
    std::vector<bool> data = {true};
    for (int i = 0; i < 3; i++)
    {
        data.push_back(false);
    }
    altar_room_shuffler.SetData(data);
}

void generation::RoomLayoutGenerator::GenerateRooms(const RoomLayoutGenerationSettings &settings, std::shared_ptr<GameObject> root)
{
    rooms.clear();
    rooms_ordered.clear();

    auto start_point = glm::ivec2(0, 0);
    for (int i = 0; i < settings.branch_division_count; i++)
    {
        glm::vec2 random_direction = random::RandOnArc(settings.angle, settings.span);
        random_direction *= random::RandFloat(settings.branch_division_min_length, settings.branch_division_max_length);
        glm::ivec2 end_point = start_point + glm::ivec2(glm::round(random_direction));

        GenerateRoomsBetweenPoints(start_point, end_point, settings, root);

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

        GenerateRoomsBetweenPoints(room.position, end_point, settings, root);
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

void generation::RoomLayoutGenerator::AddRoom(glm::ivec2 position, std::shared_ptr<GameObject> root)
{
    if (!rooms.contains(position))
    {
        Room room = Room(position, root);
        rooms.insert(std::make_pair(position, room));
        cout << "position.x: " << position.x << " position.y: " << position.y << endl;
        rooms_ordered.push_back(room);
    }
}

void generation::RoomLayoutGenerator::Destroy()
{
    for (auto &[position, room] : rooms)
    {
        room.Destroy();
    }
    rooms.clear();
    rooms_ordered.clear();
}

generation::RoomLayoutGenerator::RoomLayoutGenerator(json &j, std::shared_ptr<GameObject> root)
{
    this->RoomLayoutGenerator::RoomLayoutGenerator();
    for (auto &j_room : j["rooms"])
    {
        glm::ivec2 position = {j_room["position"][0], j_room["position"][1]};
        rooms.insert(std::make_pair(position, Room(j_room, root)));
    }
    this->built_rooms_ = j["built_rooms"];
}

json generation::RoomLayoutGenerator::Serialize()
{
    json j;

    for (auto &[position, room] : rooms)
    {
        j["rooms"].push_back(room.Serialize());
        j["built_rooms"] = this->built_rooms_;
    }

    return j;
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


void generation::RoomGenerator::GenerateEnemies(Room& room)
{
}

void generation::RoomGenerator::GenerateHealingSpots(Room& room)
{
}

void generation::RoomGenerator::Generate()
{

}   


bool generation::CheckGateProximity(glm::vec3 pos, Room& room, float proximity)
{
    bool up = !room.up_gate;
    bool down = !room.down_gate;
    bool right = !room.right_gate;
    bool left = !room.left_gate;

    float p2 = proximity * proximity;

    //up
    if (room.up_gate)
    {
        float len = glm::length2(pos - room.up_gate_pos);
        if (len < p2)
        {
            return false;
        }
    }
    //down
    if (room.down_gate)
    {
        float len = glm::length2(pos - room.down_gate_pos);
        if (len < p2)
        {
            return false;
        }
    }
    //right
    if (room.right_gate)
    {
        float len = glm::length2(pos - room.right_gate_pos);
        if (len < p2)
        {
            return false;
        }
    }
    //left
    if (room.left_gate)
    {
        float len = glm::length2(pos - room.left_gate_pos);
        if (len < p2)
        {
            return false;
        }
    }

    return true;
}

void generation::GenerateRoom(Room& room, RoomGenerationSettings* rgs, RoomModels* rm, RoomLayoutGenerator* rlg)
{
    switch (rgs->generated_rooms)
    {
        case 1:
        {
            GenerateFirstRoom(room, rgs, rm);
            break;
        }
        case 2:
        {
            GenerateSecondRoom(room, rgs, rm);
            break;
        }
        case 3:
        {
            GenerateThirdRoom(room, rgs, rm);
            break;
        }
        case 4:
        {
            GenerateFourthRoom(room, rgs, rm);
            break;
        }
        default:
        {
            room.is_generated = true;

            room.up_walls_idx.clear();
            room.left_walls_idx.clear();
            room.lamp_positions.clear();
            room.clutter_positions.clear();
            room.clutter_idx.clear();

            std::random_device rd;
            std::mt19937 g(rd());

            //generate upper walls
            room.width = rgs->width;
            room.height = rgs->height;

            bool is_altar = rlg->built_rooms_ > 6 && rlg->altar_room_shuffler.Pop();
            room.is_altar = is_altar;

            if (room.is_altar)
            {
                room.width = 2;
                room.height = 2;
            }

            room.up_walls_idx.reserve(rgs->width);

            if ((rm->walls.size() - 1) != 0)
            {
                for (int i = 0; i < rgs->width; i++)
                {
                    int model_idx = random::RandInt(0, rm->walls.size() - 1);
                    room.up_walls_idx.push_back(model_idx);
                }
            }
            else
            {
                for (int i = 0; i < rgs->width; i++)
                {
                    room.up_walls_idx.push_back(0);
                }
            }


            //generate left walls
            room.left_walls_idx.reserve(rgs->height);

            if (rm->walls.size() != 1)
            {
                for (int i = 0; i < rgs->height; i++)
                {
                    int model_idx = random::RandInt(0, rm->walls.size() - 1);
                    room.left_walls_idx.push_back(model_idx);
                }
            }
            else
            {
                for (int i = 0; i < rgs->height; i++)
                {
                    room.left_walls_idx.push_back(0);
                }
            }

            //generate gates
            //up

            int model_idx = 0;
            assert(rm->gates.size() != 0);
            assert(room.width != 0);
            assert(room.height != 0);
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


            // Lamps

            int lamp_num = rgs->lamps;

            if (room.is_altar)
            {
                lamp_num = 2;
            }

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

            // Left bot
            if (lamp_num > 0)
            {
                auto left_bot = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);
                left_bot += kLanternPlacement[2];
                room.lamp_positions.push_back(left_bot);
                lamp_set.erase(left_bot);
                lamp_num--;
            }

            // Above 4 - random
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

            // Clutter in corners
            std::vector<glm::vec3> clutter_vector;
            std::unordered_set<glm::vec3> clutter_set;
            for (int i = 0; i < room.width; i++)
            {
                auto top_pos = glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - 0 * kModuleSize);
                auto bot_pos = glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);

                for (int i = 0; i < 3; i++)
                {
                    if (CheckGateProximity(top_pos + kCornerTopLeft[i], room, 2.5f))
                    {
                        clutter_set.insert(top_pos + kCornerTopLeft[i]);
                    }

                    if (CheckGateProximity(top_pos + kCornerTopRight[i], room, 2.5f))
                    {
                        clutter_set.insert(top_pos + kCornerTopRight[i]);
                    }

                    if (CheckGateProximity(bot_pos + kCornerBotLeft[i], room, 2.5f))
                    {
                        clutter_set.insert(bot_pos + kCornerBotLeft[i]);
                    }

                    if (CheckGateProximity(bot_pos + kCornerBotRight[i], room, 2.5f))
                    {
                        clutter_set.insert(bot_pos + kCornerBotRight[i]);
                    }
                }
            }
            for (int i = 0; i < room.height; i++)
            {
                auto left_pos = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - i * kModuleSize);
                auto right_pos = glm::vec3(-8.0f - (room.width - 1) * kModuleSize, 0.0f, -8.0f - i * kModuleSize);

                for (int i = 0; i < 3; i++)
                {
                    if (CheckGateProximity(left_pos + kCornerTopLeft[i], room, 2.5f))
                    {
                        clutter_set.insert(left_pos + kCornerTopLeft[i]);
                    }

                    if (CheckGateProximity(left_pos + kCornerTopLeft[i], room, 2.5f))
                    {
                        clutter_set.insert(left_pos + kCornerTopLeft[i]);
                    }

                    if (CheckGateProximity(left_pos + kCornerBotLeft[i], room, 2.5f))
                    {
                        clutter_set.insert(left_pos + kCornerBotLeft[i]);
                    }

                    if (CheckGateProximity(right_pos + kCornerBotRight[i], room, 2.5f))
                    {
                        clutter_set.insert(right_pos + kCornerBotRight[i]);
                    }
                }
            }
            clutter_vector = std::vector<glm::vec3>(clutter_set.begin(), clutter_set.end());
            std::shuffle(clutter_vector.begin(), clutter_vector.end(), g);


            assert(rm->clutter.size() > 0);
            for (int i = 0; i < rgs->clutter && i < clutter_vector.size(); i++)
            {
                room.clutter_positions.push_back(clutter_vector[i]);
                room.clutter_idx.push_back(random::RandInt(0, rm->clutter.size() - 1));
            }

            //Enemies
            std::unordered_set<glm::vec3> enemies_set;
            std::vector<glm::vec3> enemies_positions;

            for (int i = 0; i < room.width; i++)
            {
                for (int j = 0; j < room.height; j++)
                {
                    for (int k = 0; k < kEnemiesPositions.size(); k++)
                    {
                        enemies_set.insert(kEnemiesPositions[k] + (glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize)));
                    }
                }
            }

            enemies_positions = std::vector<glm::vec3>(enemies_set.begin(), enemies_set.end());

            int enemies_positons_size = enemies_positions.size();

            std::shuffle(enemies_positions.begin(), enemies_positions.end(), g);

            if (enemies_positons_size)
            {
                for (int i = 0; i < rgs->enemies; i++)
                {
                    room.enemies_positions.push_back(enemies_positions[i % enemies_positions.size()]);
                    room.enemies_idx.push_back(random::RandInt(0, -1 + rm->enemies.size()));
                }
            }

            if (room.is_altar)
            {
                room.enemies_positions.clear();
                room.enemies_idx.clear();
            }

            // barells

            if (rgs->barells)
            {
                int rest_positions = enemies_positons_size - rgs->enemies;
                int barell_fit = rest_positions / rgs->barells;
                int ceil = barell_fit > 0 ? rgs->barells : rest_positions;

                for (int i = 0; i < ceil; i++)
                {
                    room.barells_positions.push_back(enemies_positions[(i + rgs->enemies - 1) % enemies_positions.size()]);
                    room.barell_idx.push_back(random::RandInt(0, -1 + rm->barrles.size()));
                }
            }

            if (room.is_altar)
            {
                room.barell_idx.clear();
                room.barells_positions.clear();
            }

            break;
        }
    }
    

    rgs->generated_rooms++;
}
    
void generation::BuildRoom(Room& room, RoomModels* rm, s_ptr<Shader> shader, RoomLayoutGenerator* rlg, s_ptr<tmp::Texture> particle_texture, s_ptr<Shader> particle_shader)
{
    room.is_built = true;

    if (rlg->rooms.contains(room.position + glm::ivec2(0, -1))) rlg->rooms[room.position + glm::ivec2(0, -1)].is_discovered = true;
    if (rlg->rooms.contains(room.position + glm::ivec2(-1, 0))) rlg->rooms[room.position + glm::ivec2(-1, 0)].is_discovered = true;
    if (rlg->rooms.contains(room.position + glm::ivec2(0, 1))) rlg->rooms[room.position + glm::ivec2(0, 1)].is_discovered = true;
    if (rlg->rooms.contains(room.position + glm::ivec2(1, 0))) rlg->rooms[room.position + glm::ivec2(1, 0)].is_discovered = true;

    switch (rlg->built_rooms_)
    {
        case 1:
        {
            for (int i = 0; i < room.width; i++)
            {
                s_ptr<GameObject> wall_up = GameObject::Create(room.walls);
                wall_up->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, -0.3f, 0.0f));
                wall_up->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
                wall_up->AddComponent(make_shared<components::MeshRenderer>(rm->walls[room.up_walls_idx[i]], shader));

                if (room.up_walls_idx[i] == 1)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch2[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch2[1];

                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
                else if (room.up_walls_idx[i] == 5)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch6[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch6[1];
                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
                else if (room.up_walls_idx[i] == 6)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch7[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch7[1];

                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
            }

            for (int i = 0; i < room.height; i++)
            {
                s_ptr<GameObject> wall_left = GameObject::Create(room.walls);
                wall_left->transform_->set_position(glm::vec3(0.0, -0.3f, -8.0f - i * kModuleSize));
                wall_left->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
                wall_left->AddComponent(make_shared<components::MeshRenderer>(rm->walls[room.left_walls_idx[i]], shader));

                if (room.left_walls_idx[i] == 1)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch2[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch2[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
                else if (room.left_walls_idx[i] == 5)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch6[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch6[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
                else if (room.left_walls_idx[i] == 6)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch7[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch7[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
            }

            for (int i = 0; i < room.width; i++)
            {
                for (int j = 0; j < room.height; j++)
                {
                    s_ptr<GameObject> floor = GameObject::Create(room.floors);
                    floor->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize));
                    floor->AddComponent(FloorRendererManager::i_->CreateFloorRenderer());
                }
            }

            //generate gates
            //up
            if (room.up_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.up_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.3f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(0.0f, 2.0f, -2.0f);
                emitter->start_velocity_ = glm::vec3(0.0f, 0.0f, 2.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //down
            if (room.down_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.down_gate_pos);
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.3f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(0.0f, 2.0f, 2.0f);
                emitter->start_velocity_ = glm::vec3(0.0f, 0.0f, -2.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //right
            if (room.right_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.right_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, 90.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.3f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(2.0f, 2.0f, 0.0f);
                emitter->start_velocity_ = glm::vec3(-2.0f, 0.0f, 0.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //left
            if (room.left_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.left_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.3f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(-2.0f, 2.0f, 0.0f);
                emitter->start_velocity_ = glm::vec3(2.0f, 0.0f, 0.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            // generate lamps

            for (auto pos : room.lamp_positions)
            {
                s_ptr<GameObject> lamp = GameObject::Create(room.lamps);
                lamp->transform_->set_position(pos);
                lamp->AddComponent(make_shared<components::MeshRenderer>(rm->lamps[0], shader));
                lamp->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::LAMPS, gPRECISION, rm->lamps_c[0], 0, lamp->transform_));
            }

            // generate clutter

            for (int i = 0; i < room.clutter_idx.size(); i++)
            {
                s_ptr<GameObject> clutter = GameObject::Create(room.clutter);
                clutter->transform_->set_position(room.clutter_positions[i]);
                clutter->transform_->set_scale(glm::vec3(1.0f, 0.5f, 1.0f));
                clutter->AddComponent(make_shared<components::MeshRenderer>(rm->clutter[room.clutter_idx[i]], shader));
                clutter->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::CLUTTER, gPRECISION, rm->clutter_c[room.clutter_idx[i]], 0, clutter->transform_));
                clutter->GetComponent<components::Collider>()->softness_ = 0.2f;
            }

            // generate enemies
            for (int i = 0; i < room.enemies_positions.size(); i++)
            {
                auto enemy = GameObject::Create(room.enemies);
                enemy->transform_->TeleportToPosition(room.enemies_positions[i]);
                enemy->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::TENTACLE, gPRECISION, res::get_model("res/models/capsule.obj"), 0, enemy->transform_));
                enemy->AddComponent(pbd::PBDManager::i_->CreateParticle(3.0f, 0.88f, enemy->transform_));
                enemy->AddComponent(HealthManager::i_->CreateHealthComponent(2.0f, MONSTER));
                enemy->AddComponent(ai::EnemyAIManager::i_->CreateEnemyAI(enemy, true));
                enemy->AddComponent(std::make_shared<components::ExpDropComponent>(250.0f));
                enemy->AddComponent(std::make_shared<components::EnemySizeManager>());
                enemy->AddComponent(std::make_shared<components::ParticleEmitter>(100, res::get_texture("res/particles/zzz.png"), particle_shader));
                auto emitter = enemy->GetComponent<components::ParticleEmitter>();
                emitter->start_position_ = {0.0f, 5.5f, 0.0f};
                emitter->emission_rate_ = 1.0f;
                emitter->life_time_ = 1.5f;
                emitter->start_acceleration_ = glm::vec3(0.0, 8.0, 0.0);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->random_rotation_ = false;

                auto enemy_mesh = GameObject::Create(enemy);
                enemy_mesh->AddComponent(make_shared<components::MeshRenderer>(rm->enemies[room.enemies_idx[i]], shader));
                enemy_mesh->transform_->set_scale(glm::vec3(0.01f));


                enemy_mesh->AddComponent(anim::AnimatorManager::i_->CreateAnimatorComponent());
                /*auto enemy_attack_1 = res::get_animation("res/enemy/enemy_test.fbx", 0, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_attack_2 = res::get_animation("res/enemy/enemy_test.fbx", 1, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_attack_3 = res::get_animation("res/enemy/enemy_test.fbx", 2, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_attack_4 = res::get_animation("res/enemy/enemy_test.fbx", 3, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_attack_5 = res::get_animation("res/enemy/enemy_test.fbx", 4, rm->enemies[room.enemies_idx[i]]->path_);

                auto enemy_death_1 = res::get_animation("res/enemy/enemy_test.fbx", 5, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_death_2 = res::get_animation("res/enemy/enemy_test.fbx", 6, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_death_3 = res::get_animation("res/enemy/enemy_test.fbx", 7, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_death_4 = res::get_animation("res/enemy/enemy_test.fbx", 8, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_death_5 = res::get_animation("res/enemy/enemy_test.fbx", 9, rm->enemies[room.enemies_idx[i]]->path_);

                auto enemy_idle_1 = res::get_animation("res/enemy/enemy_test.fbx", 10, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_idle_2 = res::get_animation("res/enemy/enemy_test.fbx", 11, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_idle_3 = res::get_animation("res/enemy/enemy_test.fbx", 12, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_idle_4 = res::get_animation("res/enemy/enemy_test.fbx", 13, rm->enemies[room.enemies_idx[i]]->path_);
                auto enemy_idle_5 = res::get_animation("res/enemy/enemy_test.fbx", 14, rm->enemies[room.enemies_idx[i]]->path_);*/

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_1", anim::AnimatorManager::i_->enemy_attack_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_2", anim::AnimatorManager::i_->enemy_attack_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_3", anim::AnimatorManager::i_->enemy_attack_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_4", anim::AnimatorManager::i_->enemy_attack_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_5", anim::AnimatorManager::i_->enemy_attack_5);

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_1", anim::AnimatorManager::i_->enemy_idle_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_2", anim::AnimatorManager::i_->enemy_idle_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_3", anim::AnimatorManager::i_->enemy_idle_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_4", anim::AnimatorManager::i_->enemy_idle_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_5", anim::AnimatorManager::i_->enemy_idle_5);

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_1", anim::AnimatorManager::i_->enemy_death_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_2", anim::AnimatorManager::i_->enemy_death_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_3", anim::AnimatorManager::i_->enemy_death_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_4", anim::AnimatorManager::i_->enemy_death_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_5", anim::AnimatorManager::i_->enemy_death_5);
            }
            break;
        }
        case 2:
        {
            for (int i = 0; i < room.width; i++)
            {
                s_ptr<GameObject> wall_up = GameObject::Create(room.walls);
                wall_up->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, -0.3f, 0.0f));
                wall_up->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
                wall_up->AddComponent(make_shared<components::MeshRenderer>(rm->walls[room.up_walls_idx[i]], shader));

                if (room.up_walls_idx[i] == 1)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch2[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch2[1];

                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
                else if (room.up_walls_idx[i] == 5)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch6[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch6[1];
                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
                else if (room.up_walls_idx[i] == 6)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch7[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch7[1];

                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
            }

            for (int i = 0; i < room.height; i++)
            {
                s_ptr<GameObject> wall_left = GameObject::Create(room.walls);
                wall_left->transform_->set_position(glm::vec3(0.0, -0.3f, -8.0f - i * kModuleSize));
                wall_left->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
                wall_left->AddComponent(make_shared<components::MeshRenderer>(rm->walls[room.left_walls_idx[i]], shader));

                if (room.left_walls_idx[i] == 1)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch2[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch2[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
                else if (room.left_walls_idx[i] == 5)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch6[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch6[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
                else if (room.left_walls_idx[i] == 6)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch7[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch7[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
            }

            for (int i = 0; i < room.width; i++)
            {
                for (int j = 0; j < room.height; j++)
                {
                    if (!room.is_altar)
                    {
                        s_ptr<GameObject> floor = GameObject::Create(room.floors);
                        floor->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize));
                        floor->AddComponent(FloorRendererManager::i_->CreateFloorRenderer());
                    }
                    else
                    {
                        s_ptr<GameObject> floor = GameObject::Create(room.floors);
                        floor->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize));
                        auto floor_model = res::get_model("res/grass_floor/grass_floor.obj");
                        floor->AddComponent(make_shared<components::MeshRenderer>(floor_model, shader));
                    }
                }
            }

            //generate gates
            //up
            if (room.up_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.up_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.4f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(0.0f, 2.0f, -2.0f);
                emitter->start_velocity_ = glm::vec3(0.0f, 0.0f, 2.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //down
            if (room.down_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.down_gate_pos);
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.5f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(0.0f, 2.0f, 2.0f);
                emitter->start_velocity_ = glm::vec3(0.0f, 0.0f, -2.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //right
            if (room.right_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.right_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, 90.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.5f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(2.0f, 2.0f, 0.0f);
                emitter->start_velocity_ = glm::vec3(-2.0f, 0.0f, 0.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //left
            if (room.left_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.left_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.5f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(-2.0f, 2.0f, 0.0f);
                emitter->start_velocity_ = glm::vec3(2.0f, 0.0f, 0.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            // generate lamps

            for (auto pos : room.lamp_positions)
            {
                s_ptr<GameObject> lamp = GameObject::Create(room.lamps);
                lamp->transform_->set_position(pos);
                lamp->AddComponent(make_shared<components::MeshRenderer>(rm->lamps[0], shader));
                lamp->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::LAMPS, gPRECISION, rm->lamps_c[0], 0, lamp->transform_));
            }

            // generate clutter

            for (int i = 0; i < room.clutter_idx.size(); i++)
            {
                s_ptr<GameObject> clutter = GameObject::Create(room.clutter);
                clutter->transform_->set_position(room.clutter_positions[i]);
                clutter->transform_->set_scale(glm::vec3(1.0f, 0.5f, 1.0f));
                clutter->AddComponent(make_shared<components::MeshRenderer>(rm->clutter[room.clutter_idx[i]], shader));
                clutter->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::CLUTTER, gPRECISION, rm->clutter_c[room.clutter_idx[i]], 0, clutter->transform_));
                clutter->GetComponent<components::Collider>()->softness_ = 0.2f;
            }

            // generate enemies
            for (int i = 0; i < room.enemies_positions.size(); i++)
            {
                auto enemy = GameObject::Create(room.enemies);
                enemy->transform_->TeleportToPosition(room.enemies_positions[i]);
                enemy->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::TENTACLE, gPRECISION, res::get_model("res/models/capsule.obj"), 0, enemy->transform_));
                enemy->AddComponent(pbd::PBDManager::i_->CreateParticle(3.0f, 0.88f, enemy->transform_));
                float hp = random::RandInt(1, 5);
                enemy->AddComponent(HealthManager::i_->CreateHealthComponent(hp, MONSTER));
                enemy->AddComponent(ai::EnemyAIManager::i_->CreateEnemyAI(enemy));
                enemy->AddComponent(std::make_shared<components::ExpDropComponent>(250.0f));
                enemy->AddComponent(std::make_shared<components::SpellSlotComponent>(components::NO_SPELL));
            

                enemy->AddComponent(std::make_shared<components::EnemySizeManager>());
                auto emitter = make_shared<components::ParticleEmitter>(1000, res::get_texture("res/textures/color.png"), particle_shader);
                emitter->emission_rate_ = 0.01f;
                emitter->life_time_ = 0.5f;
                emitter->start_color_ = { 0.05f, 0.04f, 0.02f, 1.0f };
                emitter->end_color_ = { 0.05, 0.04f, 0.02f, 0.5f };
                emitter->spawns_per_emission_ = { 5, 10 };
                emitter->start_position_displacement_ = 1.25f;
                emitter->start_position_ = { 0.0f, -0.5f, 0.0f };
                emitter->start_velocity_ = { 0.0f, 1.0f, 0.0f };
                emitter->start_acceleration_ = { 0.0f, -5.0f, 0.0f };
                emitter->start_size_ = { 0.2f, 0.6f };
                emitter->end_size_ = { 0.2f, 0.2f };
                enemy->AddComponent(emitter);

                auto enemy_mesh = GameObject::Create(enemy);
                enemy_mesh->AddComponent(make_shared<components::MeshRenderer>(rm->enemies[room.enemies_idx[i]], shader));
                enemy_mesh->transform_->set_scale(glm::vec3(0.01f));


                enemy_mesh->AddComponent(anim::AnimatorManager::i_->CreateAnimatorComponent());

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_1", anim::AnimatorManager::i_->enemy_attack_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_2", anim::AnimatorManager::i_->enemy_attack_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_3", anim::AnimatorManager::i_->enemy_attack_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_4", anim::AnimatorManager::i_->enemy_attack_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_5", anim::AnimatorManager::i_->enemy_attack_5);

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_1", anim::AnimatorManager::i_->enemy_idle_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_2", anim::AnimatorManager::i_->enemy_idle_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_3", anim::AnimatorManager::i_->enemy_idle_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_4", anim::AnimatorManager::i_->enemy_idle_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_5", anim::AnimatorManager::i_->enemy_idle_5);

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_1", anim::AnimatorManager::i_->enemy_death_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_2", anim::AnimatorManager::i_->enemy_death_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_3", anim::AnimatorManager::i_->enemy_death_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_4", anim::AnimatorManager::i_->enemy_death_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_5", anim::AnimatorManager::i_->enemy_death_5);

                auto enemy_state_display = GameObject::Create(enemy);
                auto br = BillboardRendererManager::i_->CreateRenderer(res::get_texture("res/textures/quit.png"));

                br->position_offset_ = glm::vec3(0.0, hp + 1.0f, 0.0);
                enemy_state_display->transform_->set_scale(glm::vec3(0.025f));

                enemy_state_display->AddComponent(br);

                enemy->GetComponent<components::EnemyAIComponent>()->state_machine_->billboard_renderer_ = br;
            }

            //generate barells
            for (int i = 0; i < room.barells_positions.size(); i++)
            {
                auto barell = GameObject::Create(room.barells);
                barell->transform_->TeleportToPosition(room.barells_positions[i]);
                barell->AddComponent(make_shared<components::MeshRenderer>(rm->barrles[room.barell_idx[i]], shader));
                barell->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::BARREL, gPRECISION, rm->barrles[room.barell_idx[i]], 0, barell->transform_));
                barell->AddComponent(pbd::PBDManager::i_->CreateParticle(5.0f, 0.78f, barell->transform_));
                barell->AddComponent(HealthManager::i_->CreateHealthComponent(1.0f, BARELL));
                barell->AddComponent(std::make_shared<components::HpDropComponent>(10.0f));
                barell->AddComponent(ai::EnemyAIManager::i_->CreateEnemyAI(barell, true));

            }

            break;
        }
        case 3:
        {
            for (int i = 0; i < room.width; i++)
            {
                s_ptr<GameObject> wall_up = GameObject::Create(room.walls);
                wall_up->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, -0.3f, 0.0f));
                wall_up->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
                wall_up->AddComponent(make_shared<components::MeshRenderer>(rm->walls[room.up_walls_idx[i]], shader));

                if (room.up_walls_idx[i] == 1)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch2[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch2[1];

                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
                else if (room.up_walls_idx[i] == 5)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch6[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch6[1];
                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
                else if (room.up_walls_idx[i] == 6)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch7[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch7[1];

                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
            }

            for (int i = 0; i < room.height; i++)
            {
                s_ptr<GameObject> wall_left = GameObject::Create(room.walls);
                wall_left->transform_->set_position(glm::vec3(0.0, -0.3f, -8.0f - i * kModuleSize));
                wall_left->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
                wall_left->AddComponent(make_shared<components::MeshRenderer>(rm->walls[room.left_walls_idx[i]], shader));

                if (room.left_walls_idx[i] == 1)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch2[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch2[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
                else if (room.left_walls_idx[i] == 5)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch6[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch6[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
                else if (room.left_walls_idx[i] == 6)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch7[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch7[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
            }

            for (int i = 0; i < room.width; i++)
            {
                for (int j = 0; j < room.height; j++)
                {
                    if (!room.is_altar)
                    {
                        s_ptr<GameObject> floor = GameObject::Create(room.floors);
                        floor->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize));
                        floor->AddComponent(FloorRendererManager::i_->CreateFloorRenderer());
                    }
                    else
                    {
                        s_ptr<GameObject> floor = GameObject::Create(room.floors);
                        floor->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize));
                        auto floor_model = res::get_model("res/grass_floor/grass_floor.obj");
                        floor->AddComponent(make_shared<components::MeshRenderer>(floor_model, shader));
                    }
                }
            }

            //generate gates
            //up
            if (room.up_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.up_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.4f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(0.0f, 2.0f, -2.0f);
                emitter->start_velocity_ = glm::vec3(0.0f, 0.0f, 2.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //down
            if (room.down_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.down_gate_pos);
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.5f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(0.0f, 2.0f, 2.0f);
                emitter->start_velocity_ = glm::vec3(0.0f, 0.0f, -2.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //right
            if (room.right_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.right_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, 90.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.5f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(2.0f, 2.0f, 0.0f);
                emitter->start_velocity_ = glm::vec3(-2.0f, 0.0f, 0.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //left
            if (room.left_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.left_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.5f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(-2.0f, 2.0f, 0.0f);
                emitter->start_velocity_ = glm::vec3(2.0f, 0.0f, 0.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            // generate lamps

            for (auto pos : room.lamp_positions)
            {
                s_ptr<GameObject> lamp = GameObject::Create(room.lamps);
                lamp->transform_->set_position(pos);
                lamp->AddComponent(make_shared<components::MeshRenderer>(rm->lamps[0], shader));
                lamp->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::LAMPS, gPRECISION, rm->lamps_c[0], 0, lamp->transform_));
            }

            // generate clutter

            for (int i = 0; i < room.clutter_idx.size(); i++)
            {
                s_ptr<GameObject> clutter = GameObject::Create(room.clutter);
                clutter->transform_->set_position(room.clutter_positions[i]);
                clutter->transform_->set_scale(glm::vec3(1.0f, 0.5f, 1.0f));
                clutter->AddComponent(make_shared<components::MeshRenderer>(rm->clutter[room.clutter_idx[i]], shader));
                clutter->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::CLUTTER, gPRECISION, rm->clutter_c[room.clutter_idx[i]], 0, clutter->transform_));
                clutter->GetComponent<components::Collider>()->softness_ = 0.2f;
            }

            // generate enemies
            for (int i = 0; i < room.enemies_positions.size(); i++)
            {
                auto enemy = GameObject::Create(room.enemies);
                enemy->transform_->TeleportToPosition(room.enemies_positions[i]);
                enemy->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::TENTACLE, gPRECISION, res::get_model("res/models/capsule.obj"), 0, enemy->transform_));
                enemy->AddComponent(pbd::PBDManager::i_->CreateParticle(3.0f, 0.88f, enemy->transform_));
                float hp = random::RandInt(1, 5);
                if (i == 0)
                {
                    enemy->AddComponent(HealthManager::i_->CreateHealthComponent(1.0f, MONSTER));
                    hp = 1;
                }
                else
                {
                    enemy->AddComponent(HealthManager::i_->CreateHealthComponent(hp, MONSTER));
                }
                enemy->AddComponent(ai::EnemyAIManager::i_->CreateEnemyAI(enemy));
                enemy->AddComponent(std::make_shared<components::ExpDropComponent>(167.0f));
                enemy->AddComponent(std::make_shared<components::SpellSlotComponent>(components::NO_SPELL));
                if (i == 0)
                {
                    enemy->GetComponent<components::SpellSlotComponent>()->type_ = SKULL_MINION;
                }
            
                enemy->AddComponent(std::make_shared<components::EnemySizeManager>());
                auto emitter = make_shared<components::ParticleEmitter>(1000, res::get_texture("res/textures/color.png"), particle_shader);
                emitter->emission_rate_ = 0.01f;
                emitter->life_time_ = 0.5f;
                emitter->start_color_ = { 0.05f, 0.04f, 0.02f, 1.0f };
                emitter->end_color_ = { 0.05, 0.04f, 0.02f, 0.5f };
                emitter->spawns_per_emission_ = { 5, 10 };
                emitter->start_position_displacement_ = 1.25f;
                emitter->start_position_ = { 0.0f, -0.5f, 0.0f };
                emitter->start_velocity_ = { 0.0f, 1.0f, 0.0f };
                emitter->start_acceleration_ = { 0.0f, -5.0f, 0.0f };
                emitter->start_size_ = { 0.2f, 0.6f };
                emitter->end_size_ = { 0.2f, 0.2f };
                enemy->AddComponent(emitter);

                auto enemy_mesh = GameObject::Create(enemy);
                enemy_mesh->AddComponent(make_shared<components::MeshRenderer>(rm->enemies[room.enemies_idx[i]], shader));
                enemy_mesh->transform_->set_scale(glm::vec3(0.01f));


                enemy_mesh->AddComponent(anim::AnimatorManager::i_->CreateAnimatorComponent());

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_1", anim::AnimatorManager::i_->enemy_attack_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_2", anim::AnimatorManager::i_->enemy_attack_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_3", anim::AnimatorManager::i_->enemy_attack_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_4", anim::AnimatorManager::i_->enemy_attack_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_5", anim::AnimatorManager::i_->enemy_attack_5);

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_1", anim::AnimatorManager::i_->enemy_idle_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_2", anim::AnimatorManager::i_->enemy_idle_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_3", anim::AnimatorManager::i_->enemy_idle_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_4", anim::AnimatorManager::i_->enemy_idle_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_5", anim::AnimatorManager::i_->enemy_idle_5);

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_1", anim::AnimatorManager::i_->enemy_death_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_2", anim::AnimatorManager::i_->enemy_death_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_3", anim::AnimatorManager::i_->enemy_death_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_4", anim::AnimatorManager::i_->enemy_death_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_5", anim::AnimatorManager::i_->enemy_death_5);

                auto enemy_state_display = GameObject::Create(enemy);
                auto br = BillboardRendererManager::i_->CreateRenderer(res::get_texture("res/textures/quit.png"));

                br->position_offset_ = glm::vec3(0.0, hp + 1.0f, 0.0);
                enemy_state_display->transform_->set_scale(glm::vec3(0.025f));

                enemy_state_display->AddComponent(br);

                enemy->GetComponent<components::EnemyAIComponent>()->state_machine_->billboard_renderer_ = br;

                if (i > 0)
                {
                    enemy->GetComponent<components::EnemyAIComponent>()->active_ = false;

                    Timer::AddTimer(3.0f, [enemy] 
                        {
                            if (enemy != nullptr)
                            {
                                if (enemy->GetComponent<components::EnemyAIComponent>() != nullptr)
                                {
                                    enemy->GetComponent<components::EnemyAIComponent>()->active_ = true;
                                }
                            }
                        });
                }
            }

            break;
        }
        default:
        {
            for (int i = 0; i < room.width; i++)
            {
                s_ptr<GameObject> wall_up = GameObject::Create(room.walls);
                wall_up->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, -0.3f, 0.0f));
                wall_up->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
                wall_up->AddComponent(make_shared<components::MeshRenderer>(rm->walls[room.up_walls_idx[i]], shader));

                if (room.up_walls_idx[i] == 1)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch2[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch2[1];

                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
                else if (room.up_walls_idx[i] == 5)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch6[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch6[1];
                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
                else if (room.up_walls_idx[i] == 6)
                {
                    auto ul = wall_up->transform_->get_model_matrix() * kGrassPatch7[0];
                    auto dr = wall_up->transform_->get_model_matrix() * kGrassPatch7[1];

                    auto renderer = GameObject::Create();
                    renderer->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                    room.floors->transform_->AddChild(renderer->transform_);
                }
            }

            for (int i = 0; i < room.height; i++)
            {
                s_ptr<GameObject> wall_left = GameObject::Create(room.walls);
                wall_left->transform_->set_position(glm::vec3(0.0, -0.3f, -8.0f - i * kModuleSize));
                wall_left->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
                wall_left->AddComponent(make_shared<components::MeshRenderer>(rm->walls[room.left_walls_idx[i]], shader));

                if (room.left_walls_idx[i] == 1)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch2[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch2[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
                else if (room.left_walls_idx[i] == 5)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch6[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch6[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
                else if (room.left_walls_idx[i] == 6)
                {
                    auto tmp_ul = wall_left->transform_->get_model_matrix() * kGrassPatch7[0];
                    auto tmp_dr = wall_left->transform_->get_model_matrix() * kGrassPatch7[1];

                    auto ul = glm::vec3(tmp_ul.x, 0.0, tmp_dr.z);
                    auto dr = glm::vec3(tmp_dr.x, 0.0, tmp_ul.z);

                    wall_left->AddComponent(GrassRendererManager::i_->CreateRenderer(ul, dr, 30));
                }
            }

            for (int i = 0; i < room.width; i++)
            {
                for (int j = 0; j < room.height; j++)
                {
                    if (!room.is_altar)
                    {
                        s_ptr<GameObject> floor = GameObject::Create(room.floors);
                        floor->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize));
                        floor->AddComponent(FloorRendererManager::i_->CreateFloorRenderer());
                    }
                    else
                    {
                        s_ptr<GameObject> floor = GameObject::Create(room.floors);
                        floor->transform_->set_position(glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize));
                        auto floor_model = res::get_model("res/grass_floor/grass_floor.obj");
                        floor->AddComponent(make_shared<components::MeshRenderer>(floor_model, shader));
                    }
                }
            }

            //generate altar

            if (room.is_altar)
            {
                auto altar = GameObject::Create(room.altar);
                altar->transform_->set_position({-16.0f, 0.0f, -16.0f});
                altar->transform_->set_scale(glm::vec3(1.5f));

                auto altar_model = res::get_model("res/models/altar/altar.obj");
                altar->transform_->set_rotation({0.0f, 180.0f, 0.0f});
                altar->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::LAMPS, gPRECISION, altar_model, 0, altar->transform_));
                altar->AddComponent(make_shared<components::MeshRenderer>(altar_model, shader));
                auto altar_texture = res::get_texture("res/textures/press_interact.png");

                auto altar_billboard = GameObject::Create(altar);
                auto billboard = BillboardRendererManager::i_->CreateRenderer(altar_texture);
                billboard->position_offset_ = {0.0f, 3.0f, 0.0f};
                billboard->active_ = false;
                altar_billboard->AddComponent(billboard);
                altar_billboard->transform_->set_scale(glm::vec3(0.1f));
                altar_billboard->AddComponent(make_shared<components::Altar>());

                auto particle_texture = res::get_texture("res/textures/flame_particle.png");
                auto particle_shader = res::get_shader("res/shaders/Particle.vert", "res/shaders/Particle.geom", "res/shaders/Particle.frag");
            
                auto altar_emitter = GameObject::Create(altar);
                auto emitter = make_shared<components::ParticleEmitter>(1000, particle_texture, particle_shader);
                emitter->emission_rate_ = 1 / 60.0f;
                emitter->start_color_ = {1.0f, 1.0f, 1.0f, 0.2f};
                altar_emitter->AddComponent(emitter);

                auto altar_emitter2 = GameObject::Create(altar);
                auto emitter2 = make_shared<components::ParticleEmitter>(1000, particle_texture, particle_shader);
                emitter2->emission_rate_ = 1 / 60.0f;
                emitter2->start_color_ = {1.0f, 1.0f, 1.0f, 0.2f};
                altar_emitter2->AddComponent(emitter2);

                auto renderer = GameObject::Create();
                renderer->AddComponent(GrassRendererManager::i_->CreateRenderer({0.0f, 0.0f, 0.0f}, {-32.0f, 0.0f, -32.0f}, 200.0f));
                room.floors->transform_->AddChild(renderer->transform_);
            }

            //generate gates
            //up
            if (room.up_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.up_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, 180.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.4f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(0.0f, 2.0f, -2.0f);
                emitter->start_velocity_ = glm::vec3(0.0f, 0.0f, 2.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //down
            if (room.down_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.down_gate_pos);
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.5f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(0.0f, 2.0f, 2.0f);
                emitter->start_velocity_ = glm::vec3(0.0f, 0.0f, -2.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //right
            if (room.right_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.right_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, 90.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.5f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(2.0f, 2.0f, 0.0f);
                emitter->start_velocity_ = glm::vec3(-2.0f, 0.0f, 0.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            //left
            if (room.left_gate)
            {
                s_ptr<GameObject> gate = GameObject::Create(room.gates);
                gate->transform_->set_position(room.left_gate_pos);
                gate->transform_->set_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::MeshRenderer>(rm->gates[0], shader));
                gate->AddComponent(make_shared<components::ParticleEmitter>(500, particle_texture, particle_shader));
                auto emitter = gate->GetComponent<components::ParticleEmitter>();
                emitter->emission_rate_ = 0.5f;
                emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                emitter->start_size_ = glm::vec2(1.0f, 1.0f);
                emitter->end_size_ = glm::vec2(1.0f, 1.0f);
                emitter->life_time_ = 1.0f;
                emitter->start_position_ = glm::vec3(-2.0f, 2.0f, 0.0f);
                emitter->start_velocity_ = glm::vec3(2.0f, 0.0f, 0.0f);
                emitter->start_position_displacement_ = 2.0f;
                emitter->emit_particles_ = false;
            }

            // generate lamps

            for (auto pos : room.lamp_positions)
            {
                s_ptr<GameObject> lamp = GameObject::Create(room.lamps);
                lamp->transform_->set_position(pos);
                lamp->AddComponent(make_shared<components::MeshRenderer>(rm->lamps[0], shader));
                lamp->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::LAMPS, gPRECISION, rm->lamps_c[0], 0, lamp->transform_));
            }

            // generate clutter

            for (int i = 0; i < room.clutter_idx.size(); i++)
            {
                s_ptr<GameObject> clutter = GameObject::Create(room.clutter);
                clutter->transform_->set_position(room.clutter_positions[i]);
                clutter->transform_->set_scale(glm::vec3(1.0f, 0.5f, 1.0f));
                clutter->AddComponent(make_shared<components::MeshRenderer>(rm->clutter[room.clutter_idx[i]], shader));
                clutter->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::CLUTTER, gPRECISION, rm->clutter_c[room.clutter_idx[i]], 0, clutter->transform_));
                clutter->GetComponent<components::Collider>()->softness_ = 0.2f;
            }

            // generate enemies
            for (int i = 0; i < room.enemies_positions.size(); i++)
            {
                auto enemy = GameObject::Create(room.enemies);
                enemy->transform_->TeleportToPosition(room.enemies_positions[i]);
                enemy->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::TENTACLE, gPRECISION, res::get_model("res/models/capsule.obj"), 0, enemy->transform_));
                enemy->AddComponent(pbd::PBDManager::i_->CreateParticle(3.0f, 0.88f, enemy->transform_));
                float hp = random::RandInt(1, 5);
                enemy->AddComponent(HealthManager::i_->CreateHealthComponent(hp, MONSTER));
                enemy->AddComponent(ai::EnemyAIManager::i_->CreateEnemyAI(enemy));
                enemy->AddComponent(std::make_shared<components::ExpDropComponent>(250.0f));
                enemy->AddComponent(std::make_shared<components::SpellSlotComponent>(components::GET_SPELL_FROM_QUEUE));
                enemy->AddComponent(std::make_shared<components::EnemySizeManager>());
                auto emitter = make_shared<components::ParticleEmitter>(1000, res::get_texture("res/textures/color.png"), particle_shader);
                emitter->emission_rate_ = 0.01f;
                emitter->life_time_ = 0.5f;
                emitter->start_color_ = {0.05f, 0.04f, 0.02f, 1.0f};
                emitter->end_color_ = {0.05, 0.04f, 0.02f, 0.5f};
                emitter->spawns_per_emission_ = {5, 10};
                emitter->start_position_displacement_ = 1.25f;
                emitter->start_position_ = {0.0f, -0.5f, 0.0f};
                emitter->start_velocity_ = {0.0f, 1.0f, 0.0f};
                emitter->start_acceleration_ = {0.0f, -5.0f, 0.0f};
                emitter->start_size_ = {0.2f, 0.6f};
                emitter->end_size_ = {0.2f, 0.2f};
                enemy->AddComponent(emitter);

                auto enemy_mesh = GameObject::Create(enemy);
                enemy_mesh->AddComponent(make_shared<components::MeshRenderer>(rm->enemies[room.enemies_idx[i]], shader));
                enemy_mesh->transform_->set_scale(glm::vec3(0.01f));


                enemy_mesh->AddComponent(anim::AnimatorManager::i_->CreateAnimatorComponent());

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_1", anim::AnimatorManager::i_->enemy_attack_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_2", anim::AnimatorManager::i_->enemy_attack_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_3", anim::AnimatorManager::i_->enemy_attack_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_4", anim::AnimatorManager::i_->enemy_attack_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Attack_5", anim::AnimatorManager::i_->enemy_attack_5);

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_1", anim::AnimatorManager::i_->enemy_idle_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_2", anim::AnimatorManager::i_->enemy_idle_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_3", anim::AnimatorManager::i_->enemy_idle_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_4", anim::AnimatorManager::i_->enemy_idle_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Idle_5", anim::AnimatorManager::i_->enemy_idle_5);

                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_1", anim::AnimatorManager::i_->enemy_death_1);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_2", anim::AnimatorManager::i_->enemy_death_2);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_3", anim::AnimatorManager::i_->enemy_death_3);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_4", anim::AnimatorManager::i_->enemy_death_4);
                enemy_mesh->GetComponent<components::Animator>()->AddAnimation("Death_5", anim::AnimatorManager::i_->enemy_death_5);

                auto enemy_state_display = GameObject::Create(enemy);
                auto br = BillboardRendererManager::i_->CreateRenderer(res::get_texture("res/textures/quit.png"));
            
                br->position_offset_ = glm::vec3(0.0, hp + 1.0f, 0.0);
                enemy_state_display->transform_->set_scale(glm::vec3(0.025f));
            
                enemy_state_display->AddComponent(br);

                enemy->GetComponent<components::EnemyAIComponent>()->state_machine_->billboard_renderer_ = br;
            }

            //generate barells
            for (int i = 0; i < room.barells_positions.size(); i++)
            {
                auto barell = GameObject::Create(room.barells);
                barell->transform_->TeleportToPosition(room.barells_positions[i]);
                barell->AddComponent(make_shared<components::MeshRenderer>(rm->barrles[room.barell_idx[i]], shader));
                barell->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::BARREL, gPRECISION, rm->barrles[room.barell_idx[i]], 0, barell->transform_));
                barell->AddComponent(pbd::PBDManager::i_->CreateParticle(5.0f, 0.78f, barell->transform_));
                barell->AddComponent(HealthManager::i_->CreateHealthComponent(1.0f, BARELL));
                barell->AddComponent(std::make_shared<components::HpDropComponent>(10.0f));
                barell->AddComponent(ai::EnemyAIManager::i_->CreateEnemyAI(barell, true));
            
            }
    
            break;
        }
        
    }

    rlg->built_rooms_++;
}

void generation::GenerateFirstRoom(Room& room, RoomGenerationSettings* rgs, RoomModels* rm)
{
    room.is_generated = true;

    room.up_walls_idx.clear();
    room.left_walls_idx.clear();
    room.lamp_positions.clear();
    room.clutter_positions.clear();
    room.clutter_idx.clear();

    std::random_device rd;
    std::mt19937 g(rd());

    //generate upper walls
    room.width = 1;
    room.height = 1;

    room.up_walls_idx.reserve(room.width);

    if ((rm->walls.size() - 1) != 0)
    {
        for (int i = 0; i < room.width; i++)
        {
            int model_idx = random::RandInt(0, rm->walls.size() - 1);
            room.up_walls_idx.push_back(model_idx);
        }
    }
    else
    {
        for (int i = 0; i < room.width; i++)
        {
            room.up_walls_idx.push_back(0);
        }
    }


    //generate left walls
    room.left_walls_idx.reserve(room.height);

    if (rm->walls.size() != 1)
    {
        for (int i = 0; i < room.height; i++)
        {
            int model_idx = random::RandInt(0, rm->walls.size() - 1);
            room.left_walls_idx.push_back(model_idx);
        }
    }
    else
    {
        for (int i = 0; i < room.height; i++)
        {
            room.left_walls_idx.push_back(0);
        }
    }

    //generate gates
    //up

    int model_idx = 0;
    assert(rm->gates.size() != 0);
    assert(room.width != 0);
    assert(room.height != 0);
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


    // Lamps

    int lamp_num = 3;
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

    
    // Right top
    if (lamp_num > 0)
    {
        auto right_top = glm::vec3(-8.0f - (room.width - 1) * kModuleSize, 0.0f, -8.0f - 0 * kModuleSize);
        right_top += kLanternPlacement[1];
        room.lamp_positions.push_back(right_top);
        lamp_set.erase(right_top);
        lamp_num--;
    }

    // Left bot
    if (lamp_num > 0)
    {
        auto left_bot = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);
        left_bot += kLanternPlacement[2];
        room.lamp_positions.push_back(left_bot);
        lamp_set.erase(left_bot);
        lamp_num--;
    }

    // Clutter in corners
    std::vector<glm::vec3> clutter_vector;
    std::unordered_set<glm::vec3> clutter_set;
    for (int i = 0; i < room.width; i++)
    {
        auto top_pos = glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - 0 * kModuleSize);
        auto bot_pos = glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);

        for (int i = 0; i < 3; i++)
        {
            if (CheckGateProximity(top_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(top_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(top_pos + kCornerTopRight[i], room, 2.5f))
            {
                clutter_set.insert(top_pos + kCornerTopRight[i]);
            }

            if (CheckGateProximity(bot_pos + kCornerBotLeft[i], room, 2.5f))
            {
                clutter_set.insert(bot_pos + kCornerBotLeft[i]);
            }

            if (CheckGateProximity(bot_pos + kCornerBotRight[i], room, 2.5f))
            {
                clutter_set.insert(bot_pos + kCornerBotRight[i]);
            }
        }
    }
    for (int i = 0; i < room.height; i++)
    {
        auto left_pos = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - i * kModuleSize);
        auto right_pos = glm::vec3(-8.0f - (room.width - 1) * kModuleSize, 0.0f, -8.0f - i * kModuleSize);

        for (int i = 0; i < 3; i++)
        {
            if (CheckGateProximity(left_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(left_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(left_pos + kCornerBotLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerBotLeft[i]);
            }

            if (CheckGateProximity(right_pos + kCornerBotRight[i], room, 2.5f))
            {
                clutter_set.insert(right_pos + kCornerBotRight[i]);
            }
        }
    }
    clutter_vector = std::vector<glm::vec3>(clutter_set.begin(), clutter_set.end());
    std::shuffle(clutter_vector.begin(), clutter_vector.end(), g);


    assert(rm->clutter.size() > 0);
    for (int i = 0; i < rgs->clutter && i < clutter_vector.size(); i++)
    {
        room.clutter_positions.push_back(clutter_vector[i]);
        room.clutter_idx.push_back(random::RandInt(0, rm->clutter.size() - 1));
    }

    //Enemies
    room.enemies_positions.push_back(glm::vec3(-8.0f, 0.0f, -8.0f));
    room.enemies_idx.push_back(random::RandInt(0, -1 + rm->enemies.size()));
}

void generation::GenerateSecondRoom(Room& room, RoomGenerationSettings* rgs, RoomModels* rm)
{
    room.is_generated = true;

    room.up_walls_idx.clear();
    room.left_walls_idx.clear();
    room.lamp_positions.clear();
    room.clutter_positions.clear();
    room.clutter_idx.clear();

    std::random_device rd;
    std::mt19937 g(rd());

    //generate upper walls
    room.width = 2;
    room.height = 1;

    room.up_walls_idx.reserve(room.width);

    if ((rm->walls.size() - 1) != 0)
    {
        for (int i = 0; i < room.width; i++)
        {
            int model_idx = random::RandInt(0, rm->walls.size() - 1);
            room.up_walls_idx.push_back(model_idx);
        }
    }
    else
    {
        for (int i = 0; i < room.width; i++)
        {
            room.up_walls_idx.push_back(0);
        }
    }


    //generate left walls
    room.left_walls_idx.reserve(room.height);

    if (rm->walls.size() != 1)
    {
        for (int i = 0; i < room.height; i++)
        {
            int model_idx = random::RandInt(0, rm->walls.size() - 1);
            room.left_walls_idx.push_back(model_idx);
        }
    }
    else
    {
        for (int i = 0; i < room.height; i++)
        {
            room.left_walls_idx.push_back(0);
        }
    }

    //generate gates
    //up

    int model_idx = 0;
    assert(rm->gates.size() != 0);
    assert(room.width != 0);
    assert(room.height != 0);
    if (room.up_gate)
    {

        room.up_gate_idx = random::RandInt(0, rm->gates.size() - 1);
        room.up_gate_wall = 0;
        room.up_gate_pos = glm::vec3(-8.0f - room.up_gate_wall * kModuleSize, 0.0f, 0.0f);
    }

    //down
    if (room.down_gate)
    {
        room.down_gate_idx = random::RandInt(0, rm->gates.size() - 1);
        room.down_gate_wall = 0;
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


    // Lamps

    int lamp_num = 4;
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

    // Right top
    if (lamp_num > 0)
    {
        auto right_top = glm::vec3(-8.0f - (room.width - 1) * kModuleSize, 0.0f, -8.0f - 0 * kModuleSize);
        right_top += kLanternPlacement[1];
        room.lamp_positions.push_back(right_top);
        lamp_set.erase(right_top);
        lamp_num--;
    }

    // Left bot
    if (lamp_num > 0)
    {
        auto left_bot = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);
        left_bot += kLanternPlacement[2];
        room.lamp_positions.push_back(left_bot);
        lamp_set.erase(left_bot);
        lamp_num--;
    }

    // Clutter in corners
    std::vector<glm::vec3> clutter_vector;
    std::unordered_set<glm::vec3> clutter_set;
    for (int i = 0; i < room.width; i++)
    {
        auto top_pos = glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - 0 * kModuleSize);
        auto bot_pos = glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);

        for (int i = 0; i < 3; i++)
        {
            if (CheckGateProximity(top_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(top_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(top_pos + kCornerTopRight[i], room, 2.5f))
            {
                clutter_set.insert(top_pos + kCornerTopRight[i]);
            }

            if (CheckGateProximity(bot_pos + kCornerBotLeft[i], room, 2.5f))
            {
                clutter_set.insert(bot_pos + kCornerBotLeft[i]);
            }

            if (CheckGateProximity(bot_pos + kCornerBotRight[i], room, 2.5f))
            {
                clutter_set.insert(bot_pos + kCornerBotRight[i]);
            }
        }
    }
    for (int i = 0; i < room.height; i++)
    {
        auto left_pos = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - i * kModuleSize);
        auto right_pos = glm::vec3(-8.0f - (room.width - 1) * kModuleSize, 0.0f, -8.0f - i * kModuleSize);

        for (int i = 0; i < 3; i++)
        {
            if (CheckGateProximity(left_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(left_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(left_pos + kCornerBotLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerBotLeft[i]);
            }

            if (CheckGateProximity(right_pos + kCornerBotRight[i], room, 2.5f))
            {
                clutter_set.insert(right_pos + kCornerBotRight[i]);
            }
        }
    }
    clutter_vector = std::vector<glm::vec3>(clutter_set.begin(), clutter_set.end());
    std::shuffle(clutter_vector.begin(), clutter_vector.end(), g);



    assert(rm->clutter.size() > 0);
    for (int i = 0; i < rgs->clutter && i < clutter_vector.size(); i++)
    {
        room.clutter_positions.push_back(clutter_vector[i]);
        room.clutter_idx.push_back(random::RandInt(0, rm->clutter.size() - 1));
    }

    //Enemies
    room.enemies_positions.push_back(glm::vec3(-16.0f, 0.0f, -8.0f));
    room.enemies_idx.push_back(random::RandInt(0, -1 + rm->enemies.size()));

    room.barells_positions.push_back(glm::vec3(-24.0f, 0.0f, -8.0f));
    room.barell_idx.push_back(random::RandInt(0, -1 + rm->barrles.size()));
}

void generation::GenerateThirdRoom(Room& room, RoomGenerationSettings* rgs, RoomModels* rm)
{
    room.is_generated = true;

    room.up_walls_idx.clear();
    room.left_walls_idx.clear();
    room.lamp_positions.clear();
    room.clutter_positions.clear();
    room.clutter_idx.clear();

    std::random_device rd;
    std::mt19937 g(rd());

    //generate upper walls
    room.width = 4;
    room.height = 1;

    room.up_walls_idx.reserve(room.width);

    if ((rm->walls.size() - 1) != 0)
    {
        for (int i = 0; i < room.width; i++)
        {
            int model_idx = random::RandInt(0, rm->walls.size() - 1);
            room.up_walls_idx.push_back(model_idx);
        }
    }
    else
    {
        for (int i = 0; i < room.width; i++)
        {
            room.up_walls_idx.push_back(0);
        }
    }


    //generate left walls
    room.left_walls_idx.reserve(room.height);

    if (rm->walls.size() != 1)
    {
        for (int i = 0; i < room.height; i++)
        {
            int model_idx = random::RandInt(0, rm->walls.size() - 1);
            room.left_walls_idx.push_back(model_idx);
        }
    }
    else
    {
        for (int i = 0; i < room.height; i++)
        {
            room.left_walls_idx.push_back(0);
        }
    }

    //generate gates
    //up

    int model_idx = 0;
    assert(rm->gates.size() != 0);
    assert(room.width != 0);
    assert(room.height != 0);
    if (room.up_gate)
    {

        room.up_gate_idx = random::RandInt(0, rm->gates.size() - 1);
        room.up_gate_wall = 0;
        room.up_gate_pos = glm::vec3(-8.0f - room.up_gate_wall * kModuleSize, 0.0f, 0.0f);
    }

    //down
    if (room.down_gate)
    {
        room.down_gate_idx = random::RandInt(0, rm->gates.size() - 1);
        room.down_gate_wall = 0;
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


    // Lamps

    int lamp_num = 6;
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

    // Left bot
    if (lamp_num > 0)
    {
        auto left_bot = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);
        left_bot += kLanternPlacement[2];
        room.lamp_positions.push_back(left_bot);
        lamp_set.erase(left_bot);
        lamp_num--;
    }

    // Clutter in corners
    std::vector<glm::vec3> clutter_vector;
    std::unordered_set<glm::vec3> clutter_set;
    for (int i = 0; i < room.width; i++)
    {
        auto top_pos = glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - 0 * kModuleSize);
        auto bot_pos = glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);

        for (int i = 0; i < 3; i++)
        {
            if (CheckGateProximity(top_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(top_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(top_pos + kCornerTopRight[i], room, 2.5f))
            {
                clutter_set.insert(top_pos + kCornerTopRight[i]);
            }

            if (CheckGateProximity(bot_pos + kCornerBotLeft[i], room, 2.5f))
            {
                clutter_set.insert(bot_pos + kCornerBotLeft[i]);
            }

            if (CheckGateProximity(bot_pos + kCornerBotRight[i], room, 2.5f))
            {
                clutter_set.insert(bot_pos + kCornerBotRight[i]);
            }
        }
    }
    for (int i = 0; i < room.height; i++)
    {
        auto left_pos = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - i * kModuleSize);
        auto right_pos = glm::vec3(-8.0f - (room.width - 1) * kModuleSize, 0.0f, -8.0f - i * kModuleSize);

        for (int i = 0; i < 3; i++)
        {
            if (CheckGateProximity(left_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(left_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(left_pos + kCornerBotLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerBotLeft[i]);
            }

            if (CheckGateProximity(right_pos + kCornerBotRight[i], room, 2.5f))
            {
                clutter_set.insert(right_pos + kCornerBotRight[i]);
            }
        }
    }
    clutter_vector = std::vector<glm::vec3>(clutter_set.begin(), clutter_set.end());
    std::shuffle(clutter_vector.begin(), clutter_vector.end(), g);

    assert(rm->clutter.size() > 0);
    for (int i = 0; i < rgs->clutter && i < 7; i++)
    {
        room.clutter_positions.push_back(clutter_vector[i]);
        room.clutter_idx.push_back(random::RandInt(0, rm->clutter.size() - 1));
    }

    //Enemies
    room.enemies_positions.push_back(glm::vec3(-8.0f, 0.0f, -8.0f));
    room.enemies_idx.push_back(random::RandInt(0, -1 + rm->enemies.size()));

    room.enemies_positions.push_back(glm::vec3(-24.0f, 0.0f, -8.0f));
    room.enemies_idx.push_back(random::RandInt(0, -1 + rm->enemies.size()));

    room.enemies_positions.push_back(glm::vec3(-32.0f, 0.0f, -8.0f));
    room.enemies_idx.push_back(random::RandInt(0, -1 + rm->enemies.size()));
}

void generation::GenerateFourthRoom(Room& room, RoomGenerationSettings* rgs, RoomModels* rm)
{
    room.is_generated = true;

    room.up_walls_idx.clear();
    room.left_walls_idx.clear();
    room.lamp_positions.clear();
    room.clutter_positions.clear();
    room.clutter_idx.clear();

    std::random_device rd;
    std::mt19937 g(rd());

    //generate upper walls
    room.width = rgs->width;
    room.height = rgs->height;

    room.is_altar = true;

    if (room.is_altar)
    {
        room.width = 2;
        room.height = 2;
    }

    room.up_walls_idx.reserve(rgs->width);

    if ((rm->walls.size() - 1) != 0)
    {
        for (int i = 0; i < rgs->width; i++)
        {
            int model_idx = random::RandInt(0, rm->walls.size() - 1);
            room.up_walls_idx.push_back(model_idx);
        }
    }
    else
    {
        for (int i = 0; i < rgs->width; i++)
        {
            room.up_walls_idx.push_back(0);
        }
    }


    //generate left walls
    room.left_walls_idx.reserve(rgs->height);

    if (rm->walls.size() != 1)
    {
        for (int i = 0; i < rgs->height; i++)
        {
            int model_idx = random::RandInt(0, rm->walls.size() - 1);
            room.left_walls_idx.push_back(model_idx);
        }
    }
    else
    {
        for (int i = 0; i < rgs->height; i++)
        {
            room.left_walls_idx.push_back(0);
        }
    }

    //generate gates
    //up

    int model_idx = 0;
    assert(rm->gates.size() != 0);
    assert(room.width != 0);
    assert(room.height != 0);
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


    // Lamps

    int lamp_num = rgs->lamps;

    if (room.is_altar)
    {
        lamp_num = 2;
    }

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

    // Left bot
    if (lamp_num > 0)
    {
        auto left_bot = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);
        left_bot += kLanternPlacement[2];
        room.lamp_positions.push_back(left_bot);
        lamp_set.erase(left_bot);
        lamp_num--;
    }

    // Above 4 - random
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

    // Clutter in corners
    std::vector<glm::vec3> clutter_vector;
    std::unordered_set<glm::vec3> clutter_set;
    for (int i = 0; i < room.width; i++)
    {
        auto top_pos = glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - 0 * kModuleSize);
        auto bot_pos = glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - (room.height - 1) * kModuleSize);

        for (int i = 0; i < 3; i++)
        {
            if (CheckGateProximity(top_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(top_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(top_pos + kCornerTopRight[i], room, 2.5f))
            {
                clutter_set.insert(top_pos + kCornerTopRight[i]);
            }

            if (CheckGateProximity(bot_pos + kCornerBotLeft[i], room, 2.5f))
            {
                clutter_set.insert(bot_pos + kCornerBotLeft[i]);
            }

            if (CheckGateProximity(bot_pos + kCornerBotRight[i], room, 2.5f))
            {
                clutter_set.insert(bot_pos + kCornerBotRight[i]);
            }
        }
    }
    for (int i = 0; i < room.height; i++)
    {
        auto left_pos = glm::vec3(-8.0f - 0 * kModuleSize, 0.0f, -8.0f - i * kModuleSize);
        auto right_pos = glm::vec3(-8.0f - (room.width - 1) * kModuleSize, 0.0f, -8.0f - i * kModuleSize);

        for (int i = 0; i < 3; i++)
        {
            if (CheckGateProximity(left_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(left_pos + kCornerTopLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerTopLeft[i]);
            }

            if (CheckGateProximity(left_pos + kCornerBotLeft[i], room, 2.5f))
            {
                clutter_set.insert(left_pos + kCornerBotLeft[i]);
            }

            if (CheckGateProximity(right_pos + kCornerBotRight[i], room, 2.5f))
            {
                clutter_set.insert(right_pos + kCornerBotRight[i]);
            }
        }
    }
    clutter_vector = std::vector<glm::vec3>(clutter_set.begin(), clutter_set.end());
    std::shuffle(clutter_vector.begin(), clutter_vector.end(), g);


    assert(rm->clutter.size() > 0);
    for (int i = 0; i < rgs->clutter && i < clutter_vector.size(); i++)
    {
        room.clutter_positions.push_back(clutter_vector[i]);
        room.clutter_idx.push_back(random::RandInt(0, rm->clutter.size() - 1));
    }

    //Enemies
    std::unordered_set<glm::vec3> enemies_set;
    std::vector<glm::vec3> enemies_positions;

    for (int i = 0; i < room.width; i++)
    {
        for (int j = 0; j < room.height; j++)
        {
            for (int k = 0; k < kEnemiesPositions.size(); k++)
            {
                enemies_set.insert(kEnemiesPositions[k] + (glm::vec3(-8.0f - i * kModuleSize, 0.0f, -8.0f - j * kModuleSize)));
            }
        }
    }

    enemies_positions = std::vector<glm::vec3>(enemies_set.begin(), enemies_set.end());

    int enemies_positons_size = enemies_positions.size();

    std::shuffle(enemies_positions.begin(), enemies_positions.end(), g);

    if (enemies_positons_size)
    {
        for (int i = 0; i < rgs->enemies; i++)
        {
            room.enemies_positions.push_back(enemies_positions[i % enemies_positions.size()]);
            room.enemies_idx.push_back(random::RandInt(0, -1 + rm->enemies.size()));
        }
    }

    if (room.is_altar)
    {
        room.enemies_positions.clear();
        room.enemies_idx.clear();
    }

    // barells

    if (rgs->barells)
    {
        int rest_positions = enemies_positons_size - rgs->enemies;
        int barell_fit = rest_positions / rgs->barells;
        int ceil = barell_fit > 0 ? rgs->barells : rest_positions;

        for (int i = 0; i < ceil; i++)
        {
            room.barells_positions.push_back(enemies_positions[(i + rgs->enemies - 1) % enemies_positions.size()]);
            room.barell_idx.push_back(random::RandInt(0, -1 + rm->barrles.size()));
        }
    }

    if (room.is_altar)
    {
        room.barell_idx.clear();
        room.barells_positions.clear();
    }
}

glm::ivec2 generation::GetMoveDirection(Room* room, std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2)
{
    glm::ivec2 move_direction = glm::ivec2(0);
    if (room->up_gate)
    {
        auto p1l = glm::length2(room->up_gate_pos - player_1->transform_->get_global_position());
        auto p2l = glm::length2(room->up_gate_pos - player_2->transform_->get_global_position());

        if (p1l < kGateThreshold || p2l < kGateThreshold)
        {
            // cout << "GO UP!!!" << endl;
            move_direction = glm::ivec2(0, -1);
        }

    }
    if (room->right_gate)
    {
        auto p1l = glm::length2(room->right_gate_pos - player_1->transform_->get_global_position());
        auto p2l = glm::length2(room->right_gate_pos - player_2->transform_->get_global_position());

        if (p1l < kGateThreshold || p2l < kGateThreshold)
        {
            //cout << "GO RIGHT!!!" << endl;
            move_direction = glm::ivec2(-1, 0);
        }
    }
    if (room->down_gate)
    {
        auto p1l = glm::length2(room->down_gate_pos - player_1->transform_->get_global_position());
        auto p2l = glm::length2(room->down_gate_pos - player_2->transform_->get_global_position());

        if (p1l < kGateThreshold || p2l < kGateThreshold)
        {
            //cout << "GO DOWN!!!" << endl;
            move_direction = glm::ivec2(0, 1);
        }
    }
    if (room->left_gate)
    {
        auto p1l = glm::length2(room->left_gate_pos - player_1->transform_->get_global_position());
        auto p2l = glm::length2(room->left_gate_pos - player_2->transform_->get_global_position());

        if (p1l < kGateThreshold || p2l < kGateThreshold)
        {
            //cout << "GO LEFT!!!" << endl;
            move_direction = glm::ivec2(1, 0);
        }
    }
    return move_direction;
}

void generation::ChangeRooms(Room*& room, RoomLayoutGenerator& rlg, RoomGenerationSettings& rg_settings, RoomModels& models, glm::ivec2& next_room_pos, std::shared_ptr<tmp::Texture> particle_texture,std::shared_ptr<Shader> GBufferPassShader, std::shared_ptr<Shader> ParticleShader)
{
    room->room_object->Disable();

    room = &rlg.rooms[next_room_pos];
    rlg.rooms_traversed_++;

    SkullMinionManager::i_->room_ = &rlg.rooms[next_room_pos];
    drop::DropManager::i_->room_ = &rlg.rooms[next_room_pos];
    if (room->is_built)
    {
        room->room_object->Enable();
    }
    else
    {
        if (!room->is_generated)
        {
            generation::GenerateRoom(rlg.rooms[room->position], &rg_settings, &models, &rlg);
        }

        generation::BuildRoom(*room, &models, GBufferPassShader, &rlg, particle_texture, ParticleShader);
    }

    pbd::WallConstraint walls = pbd::WallConstraint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-room->width * generation::kModuleSize, 0.0f, -room->height * generation::kModuleSize), 1.0f);
    pbd::PBDManager::i_->set_walls(walls);
}

void generation::DisplacePlayersAndRope(Room* room, glm::ivec2 move_direction, std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2, Rope& rope)
{
    //przesun graczy
    glm::vec3 new_center = glm::vec3(0.0f);
    glm::vec3 player_1_pos = glm::vec3(0.0f);
    glm::vec3 player_2_pos = glm::vec3(0.0f);
    int input_door = 10 * move_direction.x + move_direction.y;

    switch (input_door)
    {
    case -1: // wychodzi gora wychodzi dolem
        new_center = room->down_gate_pos + glm::vec3(0.0f, 0.0f, 1.0f) * generation::kGateThreshold * 1.1f;
        player_1->transform_->TeleportToPosition(new_center + glm::vec3(1.0f, 0.0f, 0.0f));
        player_2->transform_->TeleportToPosition(new_center - glm::vec3(1.0f, 0.0f, 0.0f));

        break;
    case -10: // wychodzi od prawej wychodzi od lewej
        new_center = room->left_gate_pos + glm::vec3(-1.0f, 0.0f, 0.0f) * generation::kGateThreshold * 1.1f;
        player_1->transform_->TeleportToPosition(new_center + glm::vec3(0.0f, 0.0f, 1.0f));
        player_2->transform_->TeleportToPosition(new_center - glm::vec3(0.0f, 0.0f, 1.0f));
        break;
    case 1: // wychodzi do�em wychodzi gora
        new_center = room->up_gate_pos + glm::vec3(0.0f, 0.0f, -1.0f) * generation::kGateThreshold * 1.1f;
        player_1->transform_->TeleportToPosition(new_center - glm::vec3(1.0f, 0.0f, 0.0f));
        player_2->transform_->TeleportToPosition(new_center + glm::vec3(1.0f, 0.0f, 0.0f));
        break;
    case 10: // wychodzi od lewej wchodzi od prawej
        new_center = room->right_gate_pos + glm::vec3(1.0f, 0.0f, 0.0f) * generation::kGateThreshold * 1.1f;
        player_1->transform_->TeleportToPosition(new_center + glm::vec3(0.0f, 0.0f, 1.0f));
        player_2->transform_->TeleportToPosition(new_center - glm::vec3(0.0f, 0.0f, 1.0f));
        break;
    }

    //przesun line
    auto player_distance = glm::distance(player_1->transform_->get_position(), player_2->transform_->get_position());

    glm::vec3 player_dir = glm::vec3(0.0f);
    if (player_distance != 0.0f)
    {
        glm::vec3 player_dir = glm::normalize(player_2->transform_->get_position() - player_1->transform_->get_position());
    }
    float step = player_distance / (float)rope.Size();

    int rope_displacement_iterator = 0;
    for (auto& segment : rope.rope_segments_)
    {
        segment->transform_->TeleportToPosition(player_1->transform_->get_position() + player_dir * step * (float)rope_displacement_iterator);
        rope_displacement_iterator++;
    }
}