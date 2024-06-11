#ifndef ROOMGENERATOR_H
#define ROOMGENERATOR_H

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "nlohmann/json.hpp"

#include <map>
#include <deque>
#include <array>
#include <limits>
#include <random>
#include <memory>
#include <random>
#include <vector>
#include <set>

#include "../Random.h"
#include "unordered_set"
#include "../Model.h"
#include "../GameObject.h"
#include "../components/MeshRenderer.h"
#include "../components/Component.h"
#include "../components/HealthComponent.h"
#include "../components/HpDropComponent.h"
#include "../components/ExpDropComponent.h"
#include "../components/SpellSlotComponent.h"
#include "../components/ParticleEmitter.h"
#include "../collisions/Collider.h"
#include "../collisions/collisions.h"
#include "../collisions/CollisionManager.h"
#include "../ai/EnemyAIManager.h"
#include "../HealthManager.h"
#include "../GameObject.h"
#include "../physics/Rope.h"
#include "Room.h"
#include "../GrassRendererManager.h"
#include "../typedef.h"
#include "../Texture.h"

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


    const std::vector<glm::vec4> kGrassPatch2
    {
        glm::vec4(2.911f, 0.0, -3.47f, 1.0f),
        glm::vec4(8.2893f, 0.0, -0.20745f, 1.0f)
    };

    const std::vector<glm::vec4> kGrassPatch6
    {
        glm::vec4(3.211f, 0.0, -3.47f, 1.0f),
        glm::vec4(8.2893f, 0.0, -0.20745f, 1.0f)
    };

    const std::vector<glm::vec4> kGrassPatch7
    {
        glm::vec4(-7.57953f, 0.0, -3.4757f, 1.0f),
        glm::vec4(-2.55601f, 0.0, -0.20745f, 1.0f)
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
        int barells = 0;
        int enemies = 1;
        int generated_rooms = 0;
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

        int built_rooms_ = 0;

        RoomLayoutGenerator() = default;
        void GenerateRooms(const RoomLayoutGenerationSettings& settings, std::shared_ptr<GameObject> root);
        void GenerateGates();
        void AddRoom(glm::ivec2 position, std::shared_ptr<GameObject> root);

        void Destroy();

        RoomLayoutGenerator(json &j, std::shared_ptr<GameObject> root);
        json Serialize();
    };

    
    
    struct RoomModels
    {
        std::vector<s_ptr<Model>> walls;
        std::vector<s_ptr<Model>> floors;
        std::vector<s_ptr<Model>> gates;
        std::vector<s_ptr<Model>> lamps;
        std::vector<s_ptr<Model>> lamps_c;
        std::vector<s_ptr<Model>> clutter;
        std::vector<s_ptr<Model>> clutter_c;
        std::vector<s_ptr<Model>> enemies;
        std::vector<s_ptr<Model>> barrles;
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

    void GenerateFirstRoom(Room& room, RoomGenerationSettings* rgs, RoomModels* rm);

    void GenerateSecondRoom(Room& room, RoomGenerationSettings* rgs, RoomModels* rm);

    void BuildRoom(Room& room, RoomModels* rm, s_ptr<Shader> shader, RoomLayoutGenerator* rlg, s_ptr<tmp::Texture> particle_texture, s_ptr<Shader> particle_shader);

    glm::ivec2 GetMoveDirection(Room* room, std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2);
    
    void ChangeRooms(Room*& room, RoomLayoutGenerator& rlg, RoomGenerationSettings& rg_settings, RoomModels& models, glm::ivec2& next_room_pos, std::shared_ptr<tmp::Texture> particle_texture, std::shared_ptr<Shader> GBufferPassShader, std::shared_ptr<Shader> ParticleShader);

    void DisplacePlayersAndRope(Room* room, glm::ivec2 move_direction, std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2, Rope& rope);

    void AttachParticleEmittersToGates(Room* room, s_ptr<tmp::Texture> particle_texture, s_ptr<Shader> particle_shader);

}; // namespace generation

#endif // !ROOMGENERATOR_H