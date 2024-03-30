#include "../headers/GameObject.h"

GameObject::GameObject()
{
	this->transform_ = std::make_shared<Components::Transform>(this);
}

void GameObject::Update()
{
	for (auto& component : components_)
	{
		component.second->Update();
	}
}

void GameObject::PropagateUpdate()
{
	Update();
	for (auto& child : transform_->children_)
	{
		child->game_object_->PropagateUpdate();
	}
}
