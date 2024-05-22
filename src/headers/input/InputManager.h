#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <memory>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <array>
#include "glm/glm.hpp"
#include "glfw/glfw3.h"

#include "../global.h"
#include "InputObserver.h"
#include "GamepadAxisType.h"
#include "ActionMappingType.h"
#include "../Camera.h"

namespace input
{

static const glm::vec2 axis_directions[] = {glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(-1.0f, 0.0f)};

class InputManager
{

// Singleton stuff
private:
    InputManager(GLFWwindow *window, s_ptr<llr::Camera> *camera);
    ~InputManager() = default;

    GLFWwindow *window_;
    s_ptr<llr::Camera> *camera_;

public:
    static InputManager *i_;

    static void Initialize(GLFWwindow *window, s_ptr<llr::Camera> *camera)
    {
        if (i_ == nullptr)
        {
            i_ = new InputManager(window, camera);
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
    std::unordered_map<int, std::vector<s_ptr<InputObserver>>> observers_;
    std::unordered_map<int, GLFWgamepadstate> old_gamepad_states_;
    std::unordered_map<int, bool> keyboard_state;

    std::unordered_map<int, std::unordered_map<Action, ActionMappingType>> keyboard_mappings;
    std::unordered_map<int, std::unordered_map<Action, ActionMappingType>> gamepad_mappings;

    static void JoystickStateCallback(int jid, int event);

    void UpdateGamepadState(int gamepadID);
    void UpdateKeyboardState(int gamepadID);

public:
    float deadzone_ = 0.1f;

    void AddObserver(int gamepadID, s_ptr<InputObserver> observer);
    void RemoveObserver(int gamepadID, s_ptr<InputObserver> observer);
    void NotifyAction(int gamepadID, Action action, State state);

    void Update();

    glm::vec2 SafeNormalize(glm::vec2 vector)
    {
        if (glm::length(vector) > 0.0f)
        {
            return glm::normalize(vector);
        }
        else
        {
            return glm::vec2(0.0f);
        }
    }
};

}; // namespace Input

#endif // !INPUTMANAGER_H