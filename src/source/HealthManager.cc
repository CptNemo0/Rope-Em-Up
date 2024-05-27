#include "../headers/HealthManager.h"

HealthManager* HealthManager::i_ = nullptr;


HealthManager::HealthManager()
{
	health_components_ = std::deque<std::shared_ptr<components::HealthComponent>>();
}

std::shared_ptr<components::HealthComponent> HealthManager::CreateHealthComponent(float health, HEALTH_TYPE type)
{
	auto hc = std::make_shared<components::HealthComponent>(health, type);
	AddHealthComponent(hc);
	return hc;
}

void HealthManager::AddHealthComponent(std::shared_ptr<components::HealthComponent> h)
{
	health_components_.push_back(h);
}

void HealthManager::RemoveHealthComponent(std::shared_ptr<components::HealthComponent> h)
{
	auto it = std::find(health_components_.begin(), health_components_.end(), h);
	if (it != health_components_.end())
	{
		health_components_.erase(it);
		cout << "ERASING HC" << endl;
	}
}

void HealthManager::DeathUpdate()
{
	int i = 0;
	for (auto& h : health_components_)
	{
		if (h == nullptr)
		{
			continue;
		}

		if (h->active_)
		{
			if (h->health_ <= 0.0)
			{
				h->gameObject_.lock()->Destroy();
			}
		}
		i++;
	}
}

