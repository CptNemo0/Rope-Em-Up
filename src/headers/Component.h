#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

#include "GameObject.h"

class Component
{
protected:
    Component() = default;
public:
    std::shared_ptr<GameObject> game_object_;

    virtual ~Component() {};
    virtual void Start() = 0;
    virtual void Update() = 0;
};

#endif // !COMPONENT_H