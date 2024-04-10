#include "../headers/InputManager.h"

Input::InputManager *Input::InputManager::i_ = nullptr;

Input::InputManager::InputManager(GLFWwindow *window)
    : window_(window)
{
    glfwSetJoystickCallback(JoystickStateCallback);
    old_gamepad_states_.emplace(GLFW_JOYSTICK_1, GLFWgamepadstate());
    old_gamepad_states_.emplace(GLFW_JOYSTICK_2, GLFWgamepadstate());
    for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; i++)
    {
        keyboard_state[i] = false;
    }

    observers_.emplace(GLFW_JOYSTICK_1, std::vector<std::shared_ptr<InputObserver>>());
    observers_.emplace(GLFW_JOYSTICK_2, std::vector<std::shared_ptr<InputObserver>>());

    keyboard_mappings =
    {
        {GLFW_JOYSTICK_1,
        {
            {Action::MOVE, ActionMappingType{.buttonIDs = {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D}}},
            {Action::PULL_ROPE, ActionMappingType{.buttonID = GLFW_KEY_E}}
        }},
        {GLFW_JOYSTICK_2,
        {
            {Action::MOVE, ActionMappingType{.buttonIDs = {GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT}}},
            {Action::PULL_ROPE, ActionMappingType{.buttonID = GLFW_KEY_SLASH}}
        }}
    };

    gamepad_mappings =
    {
        {GLFW_JOYSTICK_1,
        {
            {Action::MOVE, ActionMappingType{.axisType = GamepadAxisType::LEFT}},
            {Action::PULL_ROPE, ActionMappingType{.buttonID = GLFW_GAMEPAD_BUTTON_A}}
        }},
        {GLFW_JOYSTICK_2,
        {
            {Action::MOVE, ActionMappingType{.axisType = GamepadAxisType::LEFT}},
            {Action::PULL_ROPE, ActionMappingType{.buttonID = GLFW_GAMEPAD_BUTTON_A}}
        }}
    };
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
        int axis_type = static_cast<int>(gamepad_mappings[gamepadID][Action::MOVE].axisType);
        glm::vec2 new_axis_state(new_gamepad_state.axes[axis_type], new_gamepad_state.axes[axis_type + 1]);
        glm::vec2 old_axis_state(old_gamepad_states_[gamepadID].axes[axis_type], old_gamepad_states_[gamepadID].axes[axis_type + 1]);

        if (new_axis_state != old_axis_state)
        {
            NotifyAction(gamepadID, Action::MOVE, State(new_axis_state));
        }

        auto pull_rope_button = gamepad_mappings[gamepadID][Action::PULL_ROPE].buttonID;
        if (new_gamepad_state.buttons[pull_rope_button] != old_gamepad_states_[gamepadID].buttons[pull_rope_button])
        {
            NotifyAction(gamepadID, Action::PULL_ROPE, State{.button = (int)new_gamepad_state.buttons[pull_rope_button]});
        }

        old_gamepad_states_[gamepadID] = new_gamepad_state;
    }
}

void Input::InputManager::UpdateKeyboardState(int gamepadID)
{
    auto &move_action_buttons = keyboard_mappings[gamepadID][Action::MOVE].buttonIDs;
    glm::vec2 old_axis_state(0.0f);
    glm::vec2 new_axis_state(0.0f);
    for (int i = 0; i < 4; i++)
    {
        if (glfwGetKey(window_, move_action_buttons[i]) == GLFW_PRESS)
        {
            new_axis_state += axis_directions[i];
        }
        if (keyboard_state[move_action_buttons[i]])
        {
            old_axis_state += axis_directions[i];
        }
    }
    if (new_axis_state != old_axis_state)
    {
        NotifyAction(gamepadID, Action::MOVE, State(new_axis_state));
    }

    auto &pull_rope_button = keyboard_mappings[gamepadID][Action::PULL_ROPE].buttonID;
    int pull_rope_button_state = glfwGetKey(window_, pull_rope_button);
    if (pull_rope_button_state != keyboard_state[pull_rope_button])
    {
        NotifyAction(gamepadID, Action::PULL_ROPE, State{.button = pull_rope_button_state});
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
    for (int gamepadID = GLFW_JOYSTICK_1; gamepadID <= GLFW_JOYSTICK_2; gamepadID++)
    {
        if (glfwJoystickPresent(gamepadID))
        {
            UpdateGamepadState(gamepadID);
        }
        else
        {
            UpdateKeyboardState(gamepadID);
        }
    }
}