#ifndef MENU_H
#define MENU_H

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <functional>
#include <unordered_map>

#include "GameObject.h"
#include "input/InputObserver.h"
#include "components/HUDRenderer.h"

struct MenuItem
{
    MenuItem() = default;
    MenuItem(s_ptr<GameObject> object, std::function<void()> OnPress)
        : object_(object), OnPress(OnPress) {}

    s_ptr<GameObject> object_;
    std::function<void()> OnPress;
};

class Menu : public input::InputObserver
{
private:
    bool move_lock_ = false;
    s_ptr<GameObject> selection_outline_;

public:
    Menu();

    bool active_ = true;

    glm::ivec2 current_pos_ = {0, 0};
    std::unordered_map<glm::ivec2, s_ptr<MenuItem>> layout_;
    void OnAction(Action action, input::State state);
    void UpdateSelection();
};

#endif // MENU_H