#include "../headers/GameObject.h"

GameObject::GameObject()
{
	this->transform_ = std::make_shared<Components::Transform>();
}

void GameObject::Update()
{
	for (auto& component : components_)
	{
		component.second->Update();
	}
}
