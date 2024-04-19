#ifndef ENEMY_AI_MANAGER_h
#define ENEMY_AI_MANAGER_h

#include <memory>

#include "State.h"
#include "../GameObject.h"
#include "../physics/PBD.h"

namespace ai
{
	class EnemyAIManager
	{
	public:
		static EnemyAIManager* i_;
	private:
		EnemyAIManager();
		~EnemyAIManager() = default;

	public:

		std::shared_ptr<GameObject> player_1_;
		std::shared_ptr<GameObject> player_2_;

		std::shared_ptr<pbd::WallConstraint> walls_;

		int choked_tentacles_;
		int multi_chokes_;

		int choke_threshold_;
		int multi_threshold_;

		float wander_speed_;
		float pursuit_speed_;
		float evasion_speed_;
		float caution_speed_;

		float attack_damage_;
		float attack_range_;

		float sense_range_;

		static void Initialize()
		{
			if (i_ == nullptr)
			{
				i_ = new EnemyAIManager();
			}
		}

		static void Destroy()
		{
			if (i_ != nullptr)
			{
				delete i_;
				i_ = nullptr;
			}
		}
	};
}

#endif // !ENEMY_AI_MANAGER_h