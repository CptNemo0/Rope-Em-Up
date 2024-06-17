#ifndef SKULL_MINION_H
#define SKULL_MINION_H

#include "Component.h"
#include "../GameObject.h"
#include "../physics/PBD.h"
#include "HealthComponent.h"

namespace components
{
	class SkullMinion : public Component, public std::enable_shared_from_this<SkullMinion>
	{
	public:

		SkullMinion() = default;
		~SkullMinion() = default;

		s_ptr<components::Transform> transform_;
		s_ptr<components::PBDParticle> partcile_;
		s_ptr<pbd::BasicGenerator> generator_;
		s_ptr<components::HealthComponent> health_component_;
		s_ptr<GameObject> target_;

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;
	};
}



#endif // !SKULL_MINION_H