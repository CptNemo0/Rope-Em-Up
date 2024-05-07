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

#include "../global.h"
#include "../Random.h"
#include "unordered_set"
#include "../Model.h"
#include "../GameObject.h"
#include "../components/MeshRenderer.h"
#include "../components/Component.h"
#include <memory>

namespace generation
{
    const float kModuleSize = 16.0f;
    const float kOffset = 0.5f;

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

        // Positioning of gates
        int up_gate_wall;
        int right_gate_wall;
        int down_gate_wall;
        int left_gate_wall;
        
        std::unordered_set<glm::ivec2> grid;
        std::vector <glm::ivec2> enemies;
        std::vector <glm::ivec2> healing_spots;
       
        Room(glm::ivec2 position = glm::ivec2(0, 0)) : position(position) {}
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

    struct RoomGenerationSettings
    {
        int width = 2;
        int height = 2;
        int healing_spots = 1;
        int enemies = 1;
    };
    
    struct RoomModels
    {
        std::vector<s_ptr<Model>> walls;
        std::vector<s_ptr<Model>> floors;
        std::vector<s_ptr<Model>> gates;
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

    void GenerateRoom(Room& room, RoomGenerationSettings* rgs, RoomModels* rm, std::deque<w_ptr<GameObject>>& room_parts, s_ptr<GameObject> scene_root, s_ptr<Shader> shader);

}; // namespace generation

#endif // !ROOMGENERATOR_H