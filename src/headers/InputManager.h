#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "InputObserver.h"
#include "GamepadAxisType.h"

#include <memory>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glfw/glfw3.h"

namespace Input
{

class InputManager
{

// Singleton stuff
private:
    InputManager();
    ~InputManager() = default;

public:
    static InputManager *i_;

    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new InputManager();
        }
    }

    static void Destroy()
    {
        if (i_ != nullptr)
        {
            delete i_;
            i_ = nullptr;
        }
    }

// Input stuff
private:
    std::vector<std::pair<InputObserver *, int>> observers_;
    std::unordered_map<int, GLFWgamepadstate> old_gamepad_states_;

    static void JoystickStateCallback(int jid, int event);

    void UpdateGamepadState(int gamepadID);

public:
    void AddObserver(InputObserver *observer, int gamepadID);
    void RemoveObserver(InputObserver *observer);
    void NotifyAxisChange(int gamepadID, GamepadAxisType axis_type, glm::vec2 state);
    void NotifyButtonChange(int gamepadID, int buttonID, bool state);

    void Update();
};

}; // namespace Input

#endif // !INPUTMANAGER_H