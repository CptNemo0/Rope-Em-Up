#ifndef ENEMY_AI_COMPONENT
#define ENEMY_AI_COMPONENT

#include <memory>

#include "Component.h"
#include "../ai/EnemyState.h"
#include "../collisions/Raycast.h"
#include "../ChokeList.h"
#include "../Shuffler.h"
#include "../audio/AudioManager.h"
#include "../res/Resources.h"

class GameObject;

namespace components
{
	

	class EnemyAIComponent : public Component, public std::enable_shared_from_this<EnemyAIComponent>
	{
	public:
		std::shared_ptr<ai::EnemyStateMachine> state_machine_;
		bool is_passive_;
		EnemyAIComponent(std::shared_ptr<ai::EnemyStateMachine> state_machine, bool is_passive) : state_machine_(state_machine), is_passive_(is_passive){};

		void CheckChoke();

		Shuffler<s_ptr<audio::AudioBuffer>> slide_sounds_;
		unsigned int slide_timer_ = -1;
		bool slide_timer_lock_ = false;
		float volume_ = 0.5f;

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;

		json Serialize() override;
	};
}



#endif // !ENEMY_AI_COMPONENT
