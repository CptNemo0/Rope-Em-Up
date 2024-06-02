#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <functional>

#include "nlohmann/json.hpp"

#include "../global.h"

class GameObject;

class Component
{
private:
    static std::map<string, std::function<s_ptr<Component>(json&, s_ptr<GameObject>)>> component_factory;
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

    static s_ptr<Component> Deserialize(json &j, s_ptr<GameObject> go);
    virtual json Serialize() { return json(); }
};

#endif // !COMPONENT_H