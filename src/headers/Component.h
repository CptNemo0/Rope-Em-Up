#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

class GameObject;

class Component
{
protected:
    Component() = default;
public:
    std::weak_ptr<GameObject> gameObject_;
    virtual ~Component() {};
    virtual void Start() = 0;
    virtual void Update() = 0;
};

#endif // !COMPONENT_H