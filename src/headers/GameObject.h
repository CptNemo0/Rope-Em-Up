#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glad/glad.h"

#include "glm/glm.hpp"
#include <memory>
#include <unordered_map>
#include <typeindex>

#include "Texture.h"
#include "components/Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "components/Component.h"

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
	std::unordered_map<std::string, std::shared_ptr<Component>> components_;

public:
	GameObject();
	~GameObject() = default;
	std::shared_ptr<components::Transform> transform_;

	static std::shared_ptr<GameObject> Create();
	static std::shared_ptr<GameObject> Create(std::shared_ptr<GameObject> parent);

	void Update();
	void PropagateUpdate();
	void PropagateStart();
	void Destroy();

	template <typename T>
	void AddComponent(std::shared_ptr<T> component)
	{
		components_[typeid(T).name()] = component;
		component->gameObject_ = shared_from_this();
	}

	template <typename T>
	std::shared_ptr<T> GetComponent()
	{
		assert(components_.find(typeid(T).name()) != components_.end());
		return std::dynamic_pointer_cast<T>(components_[typeid(T).name()]);
	}

	// If we ever need to remove a single component from a GameObject
	// Remember to refactor Destroys so it works
};


#endif // !GAMEOBJECT_H



