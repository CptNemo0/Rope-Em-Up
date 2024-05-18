#ifndef ENEMY_AI_COMPONENT
#define ENEMY_AI_COMPONENT

#include "Component.h"
#include "../ai/EnemyState.h"
#include <memory>
#include "../collisions/Raycast.h"
#include "../ChokeList.h"
namespace components
{
	class GameObject;

	class EnemyAIComponent : public Component, public std::enable_shared_from_this<EnemyAIComponent>
	{
	public:
		std::shared_ptr<ai::EnemyStateMachine> state_machine_;

		EnemyAIComponent(std::shared_ptr<ai::EnemyStateMachine> state_machine) : state_machine_(state_machine) {};

		void CheckChoke();

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;
	};
}



#endif // !ENEMY_AI_COMPONENT
