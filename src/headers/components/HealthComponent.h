#ifndef HEALTH_COMOPONENT_H
#define HEALTH_COMOPONENT_H

#include "Component.h"
#include "../GameObject.h"
#include "../Timer.h"

namespace components
{
	const float kDamageCooldown = 2.0f;

	class HealthComponent : public Component
	{
	public:

		float max_health_;
		float health_;

		bool damage_cooldown_ = false;

		HealthComponent(float health) : health_(health), max_health_(health) {}
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
