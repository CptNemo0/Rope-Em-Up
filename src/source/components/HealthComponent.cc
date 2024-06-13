#include "../../headers/components/HealthComponent.h"
#include "../../headers/HealthManager.h"
namespace components
{
	//fraction should be from 0.0 do 1.0
	//return max_health_ * fraction
	float HealthComponent::MaxHealthFraction(float fraction)
	{
		return (max_health_ * fraction);
	}

	void HealthComponent::TakeDamage(float damage)
	{
		if (damage_cooldown_)
		{
			return;
		}

		FroceDamage(damage);
		DamageCooldown();
	}

	void HealthComponent::FroceDamage(float damage)
	{
		health_ -= damage;
		
	}

	void HealthComponent::ForceHeal(float heal)
	{
		audio::AudioManager::i_->PlaySound(res::get_sound("res/sounds/heal.wav"));
		health_ += heal;

		if (health_ > max_health_)
		{
			health_ = max_health_;
		}
	}

	void HealthComponent::DamageCooldown()
	{
		damage_cooldown_ = true;
		Timer::AddTimer(1.0f, [this]()
		{
				damage_cooldown_ = false;
		});

	}

	void HealthComponent::Start()
	{
	}

	void HealthComponent::Update()
	{
	}

	void HealthComponent::Destroy()
	{
		HealthManager::i_->RemoveHealthComponent(shared_from_this());
		std::cout << "Destroying HealthCompoent" << std::endl;
	}

    HealthComponent::HealthComponent(json &j)
    {
		this->HealthComponent::HealthComponent(j["max_health"], j["health_type"]);
		health_ = j["current_health"];
		damage_cooldown_ = j["damage_cooldown"];
    }

    json HealthComponent::Serialize()
    {
        json j;

		j["health_type"] = type_;
		j["current_health"] = health_;
		j["max_health"] = max_health_;
		j["damage_cooldown"] = damage_cooldown_;

		return j;
    }
}