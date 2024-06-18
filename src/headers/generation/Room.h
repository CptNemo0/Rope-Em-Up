#ifndef ROOM_H
#define ROOM_H

#include "glm/glm.hpp"
#include "nlohmann/json.hpp"

#include "../GameObject.h"
#include "../typedef.h"

namespace generation
{
    struct Room
    {
        std::shared_ptr<GameObject> room_object;
        std::shared_ptr<GameObject> enemies;
        std::shared_ptr<GameObject> clutter;
        std::shared_ptr<GameObject> lamps;
        std::shared_ptr<GameObject> walls;
        std::shared_ptr<GameObject> gates;
        std::shared_ptr<GameObject> floors;
        std::shared_ptr<GameObject> drops;
        std::shared_ptr<GameObject> barells;
        std::shared_ptr<GameObject> minions;
        // Values that will be generated during layout generation
        glm::ivec2 position;

        bool up_gate;
        bool right_gate;
        bool down_gate;
        bool left_gate;

        bool is_generated = false;
        bool is_built = false;
        bool is_discovered = false;

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

        //Barells 
        std::vector<glm::vec3> barells_positions;
        std::vector<int> barell_idx;
        // 
        //Enemies
        std::vector<glm::vec3> enemies_positions;
        std::vector<int> enemies_idx;

        // !Values that will be generated during room generation


        Room() = default;
        Room(glm::ivec2 position);
        Room(glm::ivec2 position, std::shared_ptr<GameObject> root);

        void Destroy();

        Room(json j, s_ptr<GameObject> root);
        json Serialize();
    };
}

#endif // !ROOM_H
