#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glad/glad.h"

#include "glm/glm.hpp"
#include <memory>
#include <unordered_map>
#include <typeindex>

#include "Texture.h"
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Component.h"

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
	std::unordered_map<std::string, std::shared_ptr<Component>> components_;

public:
	GameObject();
	std::shared_ptr<Components::Transform> transform_;

	static std::shared_ptr<GameObject> Create();
	static std::shared_ptr<GameObject> Create(std::shared_ptr<GameObject> parent);

	void Update();
	void PropagateUpdate();
	void PropagateStart();

	template <typename T>
	void AddComponent(std::shared_ptr<T> component)
	{
		components_[typeid(T).name()] = component;
		component->gameObject_ = shared_from_this();
	}

	template <typename T>
	std::shared_ptr<T> GetComponent()
	{
		return std::dynamic_pointer_cast<T>(components_[typeid(T).name()]);
	}
};


#endif // !GAMEOBJECT_H



