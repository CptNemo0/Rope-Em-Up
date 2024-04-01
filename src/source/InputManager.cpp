#include "../headers/InputManager.h"

Input::InputManager *Input::InputManager::i_ = nullptr;

Input::InputManager::InputManager()
{
    glfwSetJoystickCallback(JoystickStateCallback);
    old_gamepad_states_.emplace(GLFW_JOYSTICK_1, GLFWgamepadstate());
    old_gamepad_states_.emplace(GLFW_JOYSTICK_2, GLFWgamepadstate());
}

void Input::InputManager::JoystickStateCallback(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {
        std::cout << "Joystick connected: " << glfwGetJoystickName(jid) << std::endl;
    }
    else if (event == GLFW_DISCONNECTED)
    {
        std::cout << "A joystick disconnected" << std::endl;
    }
}

void Input::InputManager::UpdateGamepadState(int gamepadID)
{
    GLFWgamepadstate new_gamepad_state;
    if (glfwGetGamepadState(gamepadID, &new_gamepad_state))
    {
        for (int i = 0; i < GLFW_GAMEPAD_AXIS_LAST; i += 2)
        {
            glm::vec2 new_axis_state(new_gamepad_state.axes[i], new_gamepad_state.axes[i + 1]);
            glm::vec2 old_axis_state(old_gamepad_states_[gamepadID].axes[i], old_gamepad_states_[gamepadID].axes[i + 1]);

            if (new_axis_state != old_axis_state)
            {
                NotifyAxisChange(gamepadID, static_cast<GamepadAxisType>(i), new_axis_state);
            }
        }

        for (int i = 0; i < GLFW_GAMEPAD_BUTTON_LAST; i++)
        {
            if (new_gamepad_state.buttons[i] != old_gamepad_states_[gamepadID].buttons[i])
            {
                NotifyButtonChange(gamepadID, i, new_gamepad_state.buttons[i]);
            }
        }

        old_gamepad_states_[gamepadID] = new_gamepad_state;
    }
}

void Input::InputManager::UpdateKeyboardState(int gamepadID)
{
    glm::vec2 axis_state(0.0f);
    auto keys = axis_keyboard_mappings[gamepadID];
    for (int i = 0; i < 4; i++)
    {
        if (glfwGetKey(nullptr, keys[i]) == GLFW_PRESS)
        {
            axis_state += axis_directions[i];
        }
    }
}

void Input::InputManager::AddObserver(InputObserver *observer, int gamepadID)
{
    observers_.push_back(std::make_pair(observer, gamepadID));
}

void Input::InputManager::RemoveObserver(InputObserver *observer)
{
    for (auto it = observers_.begin(); it != observers_.end(); it++)
    {
        if (it->first == observer)
        {
            observers_.erase(it);
            break;
        }
    }
}

void Input::InputManager::NotifyAxisChange(int gamepadID, GamepadAxisType axis_type, glm::vec2 state)
{
    for (auto &observer : observers_)
    {
        if (observer.second == gamepadID)
        {
            observer.first->OnAxisChange(axis_type, state);
        }
    }
}

void Input::InputManager::NotifyButtonChange(int gamepadID, int buttonID, bool state)
{
    for (auto &observer : observers_)
    {
        if (observer.second == gamepadID)
        {
            observer.first->OnButtonChange(buttonID, state);
        }
    }
}

void Input::InputManager::Update()
{
    for (auto &gamepad : old_gamepad_states_)
    {
        if (glfwJoystickPresent(gamepad.first))
        {
            UpdateGamepadState(gamepad.first);
        }
        else
        {
            UpdateKeyboardState(gamepad.first);
        }
    }
}