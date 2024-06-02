#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glad/glad.h"

#include "glm/glm.hpp"
#include <memory>
#include <unordered_map>
#include <typeindex>

#include "global.h"
#include "Texture.h"
#include "components/Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "components/Component.h"

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
	std::unordered_map<string, s_ptr<Component>> components_;
	bool dirty_ = true;

public:
	GameObject();
	~GameObject() = default;
	s_ptr<components::Transform> transform_;

	static s_ptr<GameObject> Create();
	static s_ptr<GameObject> Create(s_ptr<GameObject> parent);

	void Update();
	void PropagateUpdate();
	void StartNewComponents();
	void Destroy();
	void Enable();
	void Disable();

	void AddComponent(s_ptr<Component> component)
	{
		dirty_ = true;
		components_[typeid(*component).name()] = component;
		component->gameObject_ = shared_from_this();
	}

	template <typename T>
	void RemoveComponent()
	{
		if (components_.find(typeid(T).name()) == components_.end())
		{
			return;
		}
		components_[typeid(T).name()]->Destroy();
		components_.erase(typeid(T).name());
	}

	template <typename T>
	s_ptr<T> GetComponent()
	{
		//assert(components_.find(typeid(T).name()) != components_.end());
		if (components_.find(typeid(T).name()) == components_.end())
		{
			return nullptr;
		}
		return std::dynamic_pointer_cast<T>(components_[typeid(T).name()]);
	}

	// If we ever need to remove a single component from a GameObject
	// Remember to refactor Destroys so it works

	json Serialize();

	static s_ptr<GameObject> Deserialize(json &j);
};


#endif // !GAMEOBJECT_H



