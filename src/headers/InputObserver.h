#ifndef INPUTOBSERVER_H
#define INPUTOBSERVER_H

#include "GamepadAxisType.h"

#include "glm/glm.hpp"

namespace Input
{

class InputObserver
{
public:
    virtual ~InputObserver() = default;
    virtual void OnAxisChange(GamepadAxisType axis_type, glm::vec2 state) = 0;
    virtual void OnButtonChange(int buttonID, bool state) = 0;
};

} // namespace Input

#endif // !INPUTOBSERVER_H