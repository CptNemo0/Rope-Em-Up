#ifndef HEALTH_COMOPONENT_H
#define HEALTH_COMOPONENT_H

#include "nlohmann/json.hpp"

#include "../res/Resources.h"
#include "Component.h"
#include "../components/Animator.h"
#include "../GameObject.h"
#include "../Timer.h"
#include "../audio/AudioManager.h"
#include "../typedef.h"
#include "ParticleEmitter.h"
#include "MeshRenderer.h"

enum HEALTH_TYPE
{
	PLAYER,
	MONSTER,
	BARELL,
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
		float shield_ = 0.0f;

		unsigned int damage_cooldown_timer_id_ = -1;
		unsigned int flash_timer_id_ = -1;
		bool damage_cooldown_ = false;
		bool flash_ = false;
		HEALTH_TYPE type_;

		HealthComponent(float health, HEALTH_TYPE type) : health_(health), max_health_(health), type_(type) {}
		~HealthComponent() = default;

		float MaxHealthFraction(float fraction);
		void TakeDamage(float damage);
		void FroceDamage(float damage);
		void ForceHeal(float heal);
		void AddShield(float shield);

		void DamageCooldown(float dmg);

		void Die();

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;

		HealthComponent(json &j);
		json Serialize() override;
	};
}



#endif // !HEALTH_COMOPONENT_H
