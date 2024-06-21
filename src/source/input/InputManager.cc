#include "../../headers/input/InputManager.h"

input::InputManager *input::InputManager::i_ = nullptr;

input::InputManager::InputManager(GLFWwindow *window)
    : window_(window)
{
    glfwSetJoystickCallback(JoystickStateCallback);
    old_gamepad_states_.emplace(GLFW_JOYSTICK_1, GLFWgamepadstate());
    old_gamepad_states_.emplace(GLFW_JOYSTICK_2, GLFWgamepadstate());
    for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; i++)
    {
        keyboard_state[i] = false;
    }

    observers_.emplace(GLFW_JOYSTICK_1, std::vector<s_ptr<InputObserver>>());
    observers_.emplace(GLFW_JOYSTICK_2, std::vector<s_ptr<InputObserver>>());

    keyboard_mappings =
    {
        {GLFW_JOYSTICK_1,
        {
            {Action::MOVE, ActionMappingType{.buttonIDs = {GLFW_KEY_I, GLFW_KEY_K, GLFW_KEY_J, GLFW_KEY_L}}},
            {Action::PULL_ROPE, ActionMappingType{.buttonID = GLFW_KEY_U}},
            {Action::CAST_SPELL, ActionMappingType{.buttonID = GLFW_KEY_O} },
            {Action::MENU_MOVE, ActionMappingType{.buttonIDs = {GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT}}},
            {Action::MENU_CLICK, ActionMappingType{.buttonID = GLFW_KEY_ENTER}}
        }},
        {GLFW_JOYSTICK_2,
        {
            {Action::MOVE, ActionMappingType{.buttonIDs = {GLFW_KEY_T, GLFW_KEY_G, GLFW_KEY_F, GLFW_KEY_H}}},
            {Action::PULL_ROPE, ActionMappingType{.buttonID = GLFW_KEY_Y}},
            {Action::CAST_SPELL, ActionMappingType{.buttonID = GLFW_KEY_R}},
            {Action::MENU_MOVE, ActionMappingType{.buttonIDs = {GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT}}},
            {Action::MENU_CLICK, ActionMappingType{.buttonID = GLFW_KEY_ENTER}}
        }}
    };

    gamepad_mappings =
    {
        {GLFW_JOYSTICK_1,
        {
            {Action::MOVE, ActionMappingType{.axisType = GamepadAxisType::LEFT}},
            {Action::PULL_ROPE, ActionMappingType{.axisType = GamepadAxisType::TRIGGERS}},
            {Action::CAST_SPELL, ActionMappingType{.buttonID = GLFW_GAMEPAD_BUTTON_X}},
            {Action::MENU_MOVE, ActionMappingType{.axisType = GamepadAxisType::LEFT}},
            {Action::MENU_CLICK, ActionMappingType{.buttonID = GLFW_GAMEPAD_BUTTON_X}}
        }},
        {GLFW_JOYSTICK_2,
        {
            {Action::MOVE, ActionMappingType{.axisType = GamepadAxisType::LEFT}},
            {Action::PULL_ROPE, ActionMappingType{.axisType = GamepadAxisType::TRIGGERS}},
            {Action::CAST_SPELL, ActionMappingType{.buttonID = GLFW_GAMEPAD_BUTTON_X}},
            {Action::MENU_MOVE, ActionMappingType{.axisType = GamepadAxisType::LEFT}},
            {Action::MENU_CLICK, ActionMappingType{.buttonID = GLFW_GAMEPAD_BUTTON_X}}
        }}
    };
}

void input::InputManager::JoystickStateCallback(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {
        cout << "Joystick connected: " << glfwGetJoystickName(jid) << endl;
    }
    else if (event == GLFW_DISCONNECTED)
    {
        cout << "A joystick disconnected" << endl;
    }
}

void input::InputManager::UpdateGamepadState(int gamepadID)
{
    GLFWgamepadstate new_gamepad_state;
    if (glfwGetGamepadState(gamepadID, &new_gamepad_state))
    {
        // AXIS ACTIONS
        for (int action_i = Action::MOVE; action_i < Action::AXIS_END; action_i++)
        {
            Action action = static_cast<Action>(action_i);

            int axis_type = static_cast<int>(gamepad_mappings[gamepadID][action].axisType);
            glm::vec2 new_axis_state(new_gamepad_state.axes[axis_type], new_gamepad_state.axes[axis_type + 1]);
            glm::vec2 old_axis_state(old_gamepad_states_[gamepadID].axes[axis_type], old_gamepad_states_[gamepadID].axes[axis_type + 1]);

            if (glm::length(new_axis_state) < deadzone_)
            {
                new_axis_state = glm::vec2(0.0f);
            }
            if (glm::length(old_axis_state) < deadzone_)
            {
                old_axis_state = glm::vec2(0.0f);
            }

            if (new_axis_state != old_axis_state)
            {
                NotifyAction(gamepadID, action, State(-new_axis_state));
            }
        }

        // BUTTONS ACTIONS
        for (int action_i = Action::CAST_SPELL; action_i < Action::BUTTON_END; action_i++)
        {
            Action action = static_cast<Action>(action_i);

            auto action_button = gamepad_mappings[gamepadID][action].buttonID;
            if (new_gamepad_state.buttons[action_button] != old_gamepad_states_[gamepadID].buttons[action_button])
            {
                NotifyAction(gamepadID, action, State{ .button = (int)new_gamepad_state.buttons[action_button] });
            }
        }

        // CUSTOM ACTIONS
        {
            // PULL ROPE
            int axis_type = static_cast<int>(gamepad_mappings[gamepadID][Action::PULL_ROPE].axisType);
            glm::vec2 new_axis_state = {new_gamepad_state.axes[axis_type], new_gamepad_state.axes[axis_type + 1]};
            glm::vec2 old_axis_state = {old_gamepad_states_[gamepadID].axes[axis_type], old_gamepad_states_[gamepadID].axes[axis_type + 1]};

            if (new_axis_state != old_axis_state)
            {
                NotifyAction(gamepadID, Action::PULL_ROPE, State{.axis = new_axis_state});
            }
        }

        // STATE RESET
        old_gamepad_states_[gamepadID] = new_gamepad_state;
    }
}

void input::InputManager::UpdateKeyboardState(int gamepadID)
{
    // AXIS ACTIONS
    for (int action_i = Action::MOVE; action_i < Action::AXIS_END; action_i++)
    {
        Action action = static_cast<Action>(action_i);

        auto &action_buttons = keyboard_mappings[gamepadID][action].buttonIDs;
        glm::vec2 axis_state(0.0f);
        bool key_state_changed = false;
        for (int i = 0; i < 4; i++)
        {
            bool key_state = glfwGetKey(window_, action_buttons[i]);
            if (keyboard_state[action_buttons[i]] != key_state)
            {
                keyboard_state[action_buttons[i]] = key_state;
                key_state_changed = true;
            }
            if (key_state)
            {
                axis_state += axis_directions[i];
            }
        }
        if (key_state_changed)
        {
            axis_state = SafeNormalize(axis_state);
            NotifyAction(gamepadID, action, State(axis_state));
        }
    }

    // BUTTON ACTIONS
    for (int action_i = Action::CAST_SPELL; action_i < Action::BUTTON_END; action_i++)
    {
        Action action = static_cast<Action>(action_i);

        auto& action_button = keyboard_mappings[gamepadID][action].buttonID;
        int action_button_state = glfwGetKey(window_, action_button);
        if (action_button_state != (int)keyboard_state[action_button])
        {
            keyboard_state[action_button] = action_button_state;
            NotifyAction(gamepadID, action, State{ .button = action_button_state });
        }
    }

    // CUSTOM ACTIONS
    {
        Action action = Action::PULL_ROPE;

        auto &action_button = keyboard_mappings[gamepadID][action].buttonID;
        int action_button_state = glfwGetKey(window_, action_button);
        if (action_button_state != (int)keyboard_state[action_button])
        {
            keyboard_state[action_button] = action_button_state;
            NotifyAction(gamepadID, action, State{.axis = {action_button_state, action_button_state}});
        }
    }
}

void input::InputManager::AddObserver(int gamepadID, s_ptr<InputObserver> observer)
{
    observers_[gamepadID].push_back(observer);
}

void input::InputManager::RemoveObserver(int gamepadID, s_ptr<InputObserver> observer)
{
    auto &observers = observers_[gamepadID];
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void input::InputManager::NotifyAction(int gamepadID, Action action, State state)
{
    for (int i = 0; i < observers_[gamepadID].size(); i++)
    {
        auto observer = observers_[gamepadID].at(i);
        if (observer)
        {
            observer->OnAction(action, state);
        }
    }
}

void input::InputManager::Update()
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