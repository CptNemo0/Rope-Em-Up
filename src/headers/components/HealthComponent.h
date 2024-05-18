#ifndef HEALTH_COMOPONENT_H
#define HEALTH_COMOPONENT_H

#include "Component.h"
#include "../GameObject.h"
#include "../Timer.h"

enum HEALTH_TYPE
{
	PLAYER,
	MONSTER,
	OTHER
};

namespace components
{
	const float kDamageCooldown = 2.0f;

	class HealthComponent : public Component, public std::enable_shared_from_this<HealthComponent>
	{
	public:

		float max_health_;
		float health_;

		bool damage_cooldown_ = false;
		HEALTH_TYPE type_;

		HealthComponent(float health, HEALTH_TYPE type) : health_(health), max_health_(health), type_(type) {}
		~HealthComponent() = default;

		float MaxHealthFraction(float fraction);
		void TakeDamage(float damage);
		void FroceDamage(float damage);
		void ForceHeal(float heal);

		void DamageCooldown();

		void Die();

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;
	};
}



#endif // !HEALTH_COMOPONENT_H
