#include "../headers/GameObject.h"

GameObject::GameObject()
{
	transform_ = std::make_shared<Components::Transform>();
}

std::shared_ptr<GameObject> GameObject::Create()
{
    auto game_object = std::make_shared<GameObject>();
	game_object->transform_->game_object_ = game_object;
	return game_object;
}

std::shared_ptr<GameObject> GameObject::Create(std::shared_ptr<GameObject> parent)
{
    auto game_object = Create();
	parent->transform_->AddChild(game_object->transform_);
	return game_object;
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

void GameObject::PropagateStart()
{
	for (auto& component : components_)
	{
		component.second->Start();
	}
	for (auto& child : transform_->children_)
	{
		child->game_object_->PropagateStart();
	}
}

void GameObject::Destroy()
{
	for (auto& component : components_)
	{
		component.second->Destroy();
	}
	transform_->game_object_ = nullptr;
	transform_->parent_->RemoveChild(transform_);
}
