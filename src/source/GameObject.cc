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

void GameObject::ClearComponents()
{
	for (auto& component : components_)
	{
		component.second->Destroy();
	}
	components_.clear();
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

void GameObject::PropagateDestroy()
{
	ClearComponents();
	for (auto& child : transform_->children_)
	{
		child->game_object_->PropagateDestroy();
	}
	transform_->children_.clear();
	transform_->game_object_ = nullptr;
}

void GameObject::Destroy()
{
	PropagateDestroy();
	if (transform_->parent_)
	{
		transform_->parent_->RemoveChild(transform_);
	}
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

json GameObject::Serialize()
{
    json j;

	j["transform"] = transform_->Serialize();
	
	for (auto &component : components_)
	{
		json j_comp;
		j_comp["type"] = component.first;
		j_comp["active"] = component.second->active_;
		j_comp["data"] = component.second->Serialize();

		j["components"].push_back(j_comp);
	}

	for (auto &child : transform_->children_)
	{
		json j_child = child->game_object_->Serialize();
		j["children"].push_back(j_child);
	}

	return j;
}

s_ptr<GameObject> GameObject::Deserialize(json &j)
{
    auto go = GameObject::Create();

	go->transform_->set_from_json(j["transform"]);

	if (j.contains("components"))
	{
		for (auto &j_comp : j["components"])
		{
			auto comp = Component::Deserialize(j_comp, go);
			if (comp != nullptr)
			{
				comp->active_ = j_comp["active"];
				go->AddComponent(comp);
			}
		}
	}

	if (j.contains("children"))
	{
		for (auto &j_child : j["children"])
		{
			auto child = GameObject::Deserialize(j_child);
			go->transform_->AddChild(child->transform_);
		}
	}

	return go;
}
