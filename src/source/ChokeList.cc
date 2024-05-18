#include "../headers/ChokeList.h"

ChokeList* ChokeList::i_ = nullptr;

ChokeList::ChokeList()
{
	health_components_ = std::vector<std::shared_ptr<components::HealthComponent>>();
}

void ChokeList::AddHealthComponent(std::shared_ptr<components::HealthComponent> h )
{
	auto it = std::find(health_components_.begin(), health_components_.end(), h);
	if (it == health_components_.end())
	{
		health_components_.push_back(h);
	}
}

void ChokeList::RemoveHealthComponent(std::shared_ptr<components::HealthComponent> h)
{
	auto it = std::find(health_components_.begin(), health_components_.end(), h);
	if (it != health_components_.end())
	{
		health_components_.erase(it);
	}
}

void ChokeList::Choke(float dmg)
{
	for (auto& h : health_components_)
	{
		h->TakeDamage(dmg);
	}
	RemoveDead();
}

void ChokeList::RemoveDead()
{
	for (int i = 0; i < health_components_.size(); i++)
	{
		if (health_components_[i]->health_ <= 0.0f)
		{
			health_components_.erase(health_components_.begin() + i);
			i = i - 1;
		}
	}
}
