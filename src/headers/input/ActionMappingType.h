#ifndef ACTIONMAPPINGTYPE_H
#define ACTIONMAPPINGTYPE_H

#include "glm/glm.hpp"
#include <unordered_map>
#include <array>

#include "Action.h"
#include "GamepadAxisType.h"

union ActionMappingType
{
    int buttonID;
    std::array<int, 4> buttonIDs;
    input::GamepadAxisType axisType;
};

#endif // !ACTIONMAPPINGTYPE_H