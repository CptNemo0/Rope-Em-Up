#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glad/glad.h"

#include "glm/glm.hpp"
#include <memory>

#include "Texture.h"
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"

class GameObject
{
public:
	std::shared_ptr<Transform> transform_;

	std::shared_ptr<Mesh> mesh_;
	std::shared_ptr<Texture> texture_;
	std::shared_ptr<Shader> shader_;

	GameObject() = default;
	GameObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader);

	void Render();
};


#endif // !GAMEOBJECT_H



