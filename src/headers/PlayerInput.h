#ifndef PLAYERINPUT_H
#define PLAYERINPUT_H

#include <unordered_map>
#include <array>
#include "glm/glm.hpp"

#include "Action.h"
#include "GamepadAxisType.h"

struct PlayerInput
{
    std::pair<Input::GamepadAxisType, glm::vec2> axis;
    
};

#endif // !PLAYERINPUT_H