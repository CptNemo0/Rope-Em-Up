#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{
protected:
    Component() = default;
public:
    virtual ~Component() {};
    virtual void Start() = 0;
    virtual void Update() = 0;
};

#endif // !COMPONENT_H