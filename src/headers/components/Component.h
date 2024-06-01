#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

#include "nlohmann/json.hpp"

#include "../global.h"

class GameObject;

class Component
{
protected:
    Component() = default;
public:
    bool dirty_ = true;
    bool active_ = true;
    std::weak_ptr<GameObject> gameObject_;
    virtual ~Component() {};
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;

    virtual json Serialize() { return json(); }
};

#endif // !COMPONENT_H