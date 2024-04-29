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

namespace generation
{

struct RoomGenerationSettings
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
    glm::ivec2 position;

    Room(glm::ivec2 position = glm::ivec2(0, 0)) : position(position) {}
};

class RoomGenerator
{
private:
    static inline const glm::ivec2 direction[4] = {glm::ivec2(1, 0), glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(0, -1)};
    static inline const int reverse_direction[4] = {1, 0, 3, 2};
    std::pair<int, glm::ivec2> FindNextClosestPoint(const glm::ivec2 &A, const glm::ivec2 &B, const glm::ivec2 &point, int prev_direction);
    void GenerateRoomsBetweenPoints(const glm::ivec2 &A, const glm::ivec2 &B, const RoomGenerationSettings &settings);

public:
    std::unordered_map<glm::ivec2, Room> rooms;
    std::deque<Room> rooms_ordered;

    RoomGenerator() = default;
    void GenerateRooms(const RoomGenerationSettings &settings);
    void AddRoom(glm::ivec2 position);
};

}; // namespace generation

#endif // !ROOMGENERATOR_H