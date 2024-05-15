#include "../../headers/components/HealthComponent.h"

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
		if (health_ <= 0)
		{
			Die();
		}
	}

	void HealthComponent::ForceHeal(float heal)
	{
		health_ += heal;
	}

	void HealthComponent::DamageCooldown()
	{
		damage_cooldown_ = true;
		Timer::AddTimer(1.0f, [this]()
		{
				damage_cooldown_ = false;
		});

	}

	void HealthComponent::Die()
	{
		gameObject_.lock()->Destroy();
	}

	void HealthComponent::Start()
	{
	}

	void HealthComponent::Update()
	{
	}

	void HealthComponent::Destroy()
	{
		std::cout << "Destroying HealthCompoent" << std::endl;
	}

}