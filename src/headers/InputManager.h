#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "InputObserver.h"
#include "GamepadAxisType.h"

#include <memory>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <array>
#include "glm/glm.hpp"
#include "glfw/glfw3.h"

namespace Input
{

static std::unordered_map<int, std::array<int, 4>> axis_keyboard_mappings =
{
    {GLFW_JOYSTICK_1, {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D}},
    {GLFW_JOYSTICK_2, {GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT}}
};

static const glm::vec2 axis_directions[] = {glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, -1.0f), glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 0.0f)};

static std::unordered_map<int, std::vector<int>> button_keyboard_mappings =
{
    {GLFW_JOYSTICK_1, {GLFW_KEY_E}},
    {GLFW_JOYSTICK_2, {GLFW_KEY_SLASH}}
};

class InputManager
{

// Singleton stuff
private:
    InputManager(GLFWwindow *window);
    ~InputManager() = default;

    GLFWwindow *window_;

public:
    static InputManager *i_;

    static void Initialize(GLFWwindow *window)
    {
        if (i_ == nullptr)
        {
            i_ = new InputManager(window);
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
    void UpdateKeyboardState(int gamepadID);

public:
    void AddObserver(InputObserver *observer, int gamepadID);
    void RemoveObserver(InputObserver *observer);
    void NotifyAxisChange(int gamepadID, GamepadAxisType axis_type, glm::vec2 state);
    void NotifyButtonChange(int gamepadID, int buttonID, bool state);

    void Update();
};

}; // namespace Input

#endif // !INPUTMANAGER_H