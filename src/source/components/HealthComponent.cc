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

}