#include "../headers/GameObject.h"

GameObject::GameObject()
{
	transform_ = make_shared<components::Transform>();
}

s_ptr<GameObject> GameObject::Create()
{
    auto game_object = make_shared<GameObject>();
	game_object->transform_->game_object_ = game_object;
	return game_object;
}

s_ptr<GameObject> GameObject::Create(s_ptr<GameObject> parent)
{
    auto game_object = Create();
	parent->transform_->AddChild(game_object->transform_);
	return game_object;
}

void GameObject::Update()
{
	if (dirty_)
	{
		StartNewComponents();
		dirty_ = false;
	}
	for (auto& component : components_)
	{
		if (component.second->active_)
		{
			component.second->Update();
		}
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

void GameObject::StartNewComponents()
{
	for (auto& component : components_)
	{
		if (component.second->dirty_)
		{
			component.second->Start();
			component.second->dirty_ = false;
		}
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

void GameObject::Enable()
{
	for (auto& component : components_)
	{
		component.second->active_ = true;
	}
	for (auto& child : transform_->children_)
	{
		child->game_object_->Enable();
	}
}

void GameObject::Disable()
{
	for (auto& component : components_)
	{
		component.second->active_ = false;
	}
	for (auto& child : transform_->children_)
	{
		child->game_object_->Disable();
	}
}
