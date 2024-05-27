#ifndef ROOMGENERATOR_H
#define ROOMGENERATOR_H

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <map>
#include <deque>
#include <array>
#include <limits>
#include <random>
#include <memory>
#include <random>
#include <vector>
#include <set>

#include "../global.h"
#include "../Random.h"
#include "unordered_set"
#include "../Model.h"
#include "../GameObject.h"
#include "../components/MeshRenderer.h"
#include "../components/Component.h"
#include "../components/HealthComponent.h"
#include "../collisions/Collider.h"
#include "../collisions/collisions.h"
#include "../collisions/CollisionManager.h"
#include "../ai/EnemyAIManager.h"
#include "../HealthManager.h"
#include "../GameObject.h"

namespace generation
{
    const float kModuleSize = 16.0f;
    const float kOffset = 0.5f;
    const float kGateThreshold = 4.0f;

    const std::vector<glm::vec3> kLanternPlacement
    {
        glm::vec3(4.0f, 0.0f, 4.0f),   // left top
        glm::vec3(-4.0f, 0.0f, 4.0f),  // right top
        glm::vec3(4.0f, 0.0f, -4.0f),  // left bot
        glm::vec3(-4.0f, 0.0f, -4.0f)  // right bot
    };

    const std::vector<glm::vec3> kCornerTopRight
    {
        glm::vec3(-6.0f, 0.0f, 4.0f),
        glm::vec3(-6.0f, 0.0f, 6.0f),
        glm::vec3(-4.0f, 0.0f, 6.0f),
    };

    const std::vector<glm::vec3> kCornerTopLeft
    {
        glm::vec3(4.0f, 0.0f, 6.0f),
        glm::vec3(6.0f, 0.0f, 6.0f),
        glm::vec3(6.0f, 0.0f, 4.0f),
    };

    const std::vector<glm::vec3> kCornerBotRight
    {
        glm::vec3(-6.0f, 0.0f, -4.0f),
        glm::vec3(-6.0f, 0.0f, -6.0f),
        glm::vec3(-4.0f, 0.0f, -6.0f),
    };

    const std::vector<glm::vec3> kCornerBotLeft
    {
        glm::vec3(4.0f, 0.0f, -6.0f),
        glm::vec3(6.0f, 0.0f, -6.0f),
        glm::vec3(6.0f, 0.0f, -4.0f),
    };

    const std::vector<glm::vec3> kEnemiesPositions
    {
        glm::vec3(-4.0f, 0.0f, 0.0f),

        glm::vec3(-2.0f, 0.0f, -2.0f),
        glm::vec3(-0.0f, 0.0f, -2.0f),
        glm::vec3(2.0f, 0.0f, -2.0f),

        glm::vec3(-2.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, 0.0f),

        glm::vec3(-2.0f, 0.0f, 2.0f),
        glm::vec3(-0.0f, 0.0f, 2.0f),
        glm::vec3(2.0f, 0.0f, 2.0f),

        glm::vec3(4.0f, 0.0f, 0.0f),
    };

    struct RoomLayoutGenerationSettings
    {
        float angle, span;
        int branch_division_count;
        float branch_division_min_length, branch_division_max_length;
        int sub_branch_count;
        float sub_branch_span;
        float sub_branch_min_length, sub_branch_max_length;
    };

    struct Room
    {
        std::shared_ptr<GameObject> room_object;

        // Values that will be generated during layout generation
        glm::ivec2 position;

        bool up_gate;
        bool right_gate;
        bool down_gate;
        bool left_gate;

        bool is_generated = false;

        // Values that will be generated during room generation

        int width;
        int height;

        // Indicies to model arrays
        std::vector<int> up_walls_idx;
        std::vector<int> left_walls_idx;
        int up_gate_idx;
        int right_gate_idx;
        int down_gate_idx;
        int left_gate_idx;

        // Walls of gates
        int up_gate_wall;
        int right_gate_wall;
        int down_gate_wall;
        int left_gate_wall;
        
        //Positions of gates
        glm::vec3 up_gate_pos;
        glm::vec3 right_gate_pos;
        glm::vec3 down_gate_pos;
        glm::vec3 left_gate_pos;

        //Lamp positions
        std::vector<glm::vec3> lamp_positions;
        std::vector<bool> lamp_activity;

        //Clutter
        std::vector<glm::vec3> clutter_positions;
        std::vector<int> clutter_idx;

        //Enemies
        std::vector<glm::vec3> enemies_positions;
        std::vector<int> enemies_idx;

        // !Values that will be generated during room generation

        //Static values
        static std::deque<w_ptr<GameObject>> room_parts;
        static std::vector<w_ptr<GameObject>> enemies;

        Room(glm::ivec2 position = glm::ivec2(0, 0)) : position(position) 
        {
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
    };

    static inline void CleanUpEnemiesVecotr(Room& room)
    {
        for (int i = 0; i < Room::enemies.size(); i++)
        {
            auto hc = Room::enemies[i].lock()->GetComponent<components::HealthComponent>();
            if (hc->health_ <= 0.0f)
            {
                Room::enemies.erase(Room::enemies.begin() + i);
                i = i - 1;
            }
        }
    }

    static inline void DeleteCurrentRoom(Room& room)
    {
        for (auto& a : Room::room_parts)
        {
            a.lock()->Destroy();
            a.lock() = nullptr;
        }

        for (auto& e : Room::enemies)
        {
            e.lock()->Destroy();
            e.lock() = nullptr;
        }

        Room::room_parts.clear();
        Room::enemies.clear();
    }

    static inline glm::ivec2 GetMoveDirection(Room* room, std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2)
    {
        glm::ivec2 move_direction = glm::ivec2(0);
        if (room->up_gate)
        {
            auto p1l = glm::length2(room->up_gate_pos - player_1->transform_->get_global_position());
            auto p2l = glm::length2(room->up_gate_pos - player_2->transform_->get_global_position());

            if (p1l < kGateThreshold || p2l < kGateThreshold)
            {
                cout << "GO UP!!!" << endl;
                move_direction = glm::ivec2(0, -1);
            }

        }
        if (room->right_gate)
        {
            auto p1l = glm::length2(room->right_gate_pos - player_1->transform_->get_global_position());
            auto p2l = glm::length2(room->right_gate_pos - player_2->transform_->get_global_position());

            if (p1l < kGateThreshold || p2l < kGateThreshold)
            {
                cout << "GO RIGHT!!!" << endl;
                move_direction = glm::ivec2(-1, 0);
            }
        }
        if (room->down_gate)
        {
            auto p1l = glm::length2(room->down_gate_pos - player_1->transform_->get_global_position());
            auto p2l = glm::length2(room->down_gate_pos - player_2->transform_->get_global_position());

            if (p1l < kGateThreshold || p2l < kGateThreshold)
            {
                cout << "GO DOWN!!!" << endl;
                move_direction = glm::ivec2(0, 1);
            }
        }
        if (room->left_gate)
        {
            auto p1l = glm::length2(room->left_gate_pos - player_1->transform_->get_global_position());
            auto p2l = glm::length2(room->left_gate_pos - player_2->transform_->get_global_position());

            if (p1l < kGateThreshold || p2l < kGateThreshold)
            {
                cout << "GO LEFT!!!" << endl;
                move_direction = glm::ivec2(1, 0);
            }
        }
        return move_direction;
    }

    struct RoomGenerationSettings
    {
        int width = 2;
        int height = 2;
        int lamps = 1;
        int active_lamps = 0;
        int clutter = 10;
        int enemies = 1;
    };

    class RoomLayoutGenerator
    {
    private:
        static inline const glm::ivec2 direction[4] = {glm::ivec2(1, 0), glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(0, -1)};
        static inline const int reverse_direction[4] = {1, 0, 3, 2};
        std::pair<int, glm::ivec2> FindNextClosestPoint(const glm::ivec2 &A, const glm::ivec2 &B, const glm::ivec2 &point, int prev_direction);
        void GenerateRoomsBetweenPoints(const glm::ivec2 &A, const glm::ivec2 &B, const RoomLayoutGenerationSettings &settings);

    public:
        std::unordered_map<glm::ivec2, Room> rooms;
        std::deque<Room> rooms_ordered;

        RoomLayoutGenerator() = default;
        void GenerateRooms(const RoomLayoutGenerationSettings &settings);
        void GenerateGates();
        void AddRoom(glm::ivec2 position);
    };

    
    
    struct RoomModels
    {
        std::vector<s_ptr<Model>> walls;
        std::vector<s_ptr<Model>> floors;
        std::vector<s_ptr<Model>> gates;
        std::vector<s_ptr<Model>> lamps;
        std::vector<s_ptr<Model>> clutter;
        std::vector<s_ptr<Model>> clutter_c;
        std::vector<s_ptr<Model>> enemies;
    };

    class RoomGenerator
    {
    public:
        RoomGenerationSettings settings_;

        void IncreaseSize();
        void DecreaseSize();

        void IncreaseEnemies();
        void DecreaseEnemies();

        void IncreaseHealingSpots();
        void DecreaseHealingSpots();

        void GenerateGrid(Room& room);
        void GenerateEnemies(Room& room);
        void GenerateHealingSpots(Room& room);

        void Generate();
    };

    bool CheckGateProximity(glm::vec3 pos, Room& room, float proximity);

    void GenerateRoom(Room& room, RoomGenerationSettings* rgs, RoomModels* rm);

    void BuildRoom(const Room& room, RoomModels* rm, std::deque<w_ptr<GameObject>>& room_parts, std::vector<w_ptr<GameObject>>& enemies, s_ptr<GameObject> scene_root, s_ptr<Shader> shader);

    void BuildRoom(const Room& room, RoomModels* rm, s_ptr<GameObject> scene_root, s_ptr<Shader> shader);

}; // namespace generation

#endif // !ROOMGENERATOR_H