#ifndef ENEMY_SIZE_MANAGER_H
#define ENEMY_SIZE_MANAGER_H

#include "Component.h"
#include "../GameObject.h"
#include "Transform.h"
#include "HealthComponent.h"
#include <memory>
#include <glm/glm.hpp>

#ifndef HAX_ENEMY_HP 
#define HAX_ENEMY_HP 5
#endif // ! HAX_ENEMY_HP


namespace components
{
	class EnemySizeManager : public Component
	{

	public:
		EnemySizeManager() = default;
		~EnemySizeManager() = default;

		std::shared_ptr<HealthComponent> health_component_;
		std::shared_ptr<GameObject> model_;
		int last_health_;

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;
	};
}


#endif // !ENEMY_SIZE_MANAGER_H
