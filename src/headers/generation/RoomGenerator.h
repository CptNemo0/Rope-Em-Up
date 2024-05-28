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
#include "../components/HpDropComponent.h"
#include "../collisions/Collider.h"
#include "../collisions/collisions.h"
#include "../collisions/CollisionManager.h"
#include "../ai/EnemyAIManager.h"
#include "../HealthManager.h"
#include "../GameObject.h"
#include "../physics/Rope.h"
#include "Room.h"


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
        void GenerateRoomsBetweenPoints(const glm::ivec2& A, const glm::ivec2& B, const RoomLayoutGenerationSettings& settings, std::shared_ptr<GameObject> root);

    public:
        std::unordered_map<glm::ivec2, Room> rooms;
        std::deque<Room> rooms_ordered;

        RoomLayoutGenerator() = default;
        void GenerateRooms(const RoomLayoutGenerationSettings& settings, std::shared_ptr<GameObject> root);
        void GenerateGates();
        void AddRoom(glm::ivec2 position, std::shared_ptr<GameObject> root);
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

    void BuildRoom(Room& room, RoomModels* rm, s_ptr<Shader> shader);

    glm::ivec2 GetMoveDirection(Room* room, std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2);
    
    void ChangeRooms(Room*& room, RoomLayoutGenerator& rlg, RoomGenerationSettings& rg_settings, RoomModels& models, glm::ivec2& next_room_pos, std::shared_ptr<Shader> GBufferPassShader);

    void DisplacePlayersAndRope(Room* room, glm::ivec2 move_direction, std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2, Rope& rope);

}; // namespace generation

#endif // !ROOMGENERATOR_H