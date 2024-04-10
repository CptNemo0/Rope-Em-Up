#ifndef INPUTOBSERVER_H
#define INPUTOBSERVER_H

#include "Action.h"

#include "glm/glm.hpp"

namespace Input
{

union State
{
    glm::vec2 axis;
    bool button;
};

class InputObserver
{
public:
    virtual ~InputObserver() = default;
    virtual void OnAction(Action action, State state) = 0;
};

} // namespace Input

#endif // !INPUTOBSERVER_H