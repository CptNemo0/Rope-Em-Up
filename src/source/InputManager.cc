#include "../headers/InputManager.h"

Input::InputManager *Input::InputManager::i_ = nullptr;

Input::InputManager::InputManager(GLFWwindow *window)
    : window_(window)
{
    glfwSetJoystickCallback(JoystickStateCallback);
    old_gamepad_states_.emplace(GLFW_JOYSTICK_1, GLFWgamepadstate());
    old_gamepad_states_.emplace(GLFW_JOYSTICK_2, GLFWgamepadstate());
    observers_.emplace(GLFW_JOYSTICK_1, std::vector<std::shared_ptr<InputObserver>>());
    observers_.emplace(GLFW_JOYSTICK_2, std::vector<std::shared_ptr<InputObserver>>());
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
        if (glfwGetKey(window_, keys[i]) == GLFW_PRESS)
        {
            axis_state += axis_directions[i];
        }
    }
}

void Input::InputManager::AddObserver(int gamepadID, std::shared_ptr<InputObserver> observer)
{
    observers_[gamepadID].push_back(observer);
}

void Input::InputManager::RemoveObserver(int gamepadID, std::shared_ptr<InputObserver> observer)
{
    auto &observers = observers_[gamepadID];
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Input::InputManager::NotifyAction(int gamepadID, Action action, State state)
{
    for (auto &observer : observers_[gamepadID])
    {
        observer->OnAction(action, state);
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