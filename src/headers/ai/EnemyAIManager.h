#ifndef ENEMY_AI_MANAGER_h
#define ENEMY_AI_MANAGER_h

#include <memory>

#include "State.h"
#include "../GameObject.h"
#include "../physics/PBD.h"
#include "EnemyState.h"

namespace ai
{
	struct EnemyAIManagerInitStruct
	{
		int choked_tentacles;
		int multi_chokes;

		int choke_threshold;
		int multi_threshold;

		float wall_proximity_threshold;

		float attack_damage;
		float attack_range;

		float sense_range;
	};

	class EnemyAIManager
	{
	public:
		static EnemyAIManager* i_;

	private:
		static bool players_set;
		static s_ptr<GameObject> player_1_;
		static s_ptr<GameObject> player_2_;

		EnemyAIManager(const EnemyAIManagerInitStruct& init_struct, const Vehicle& vehicle);
		~EnemyAIManager() = default;

	public:
		Vehicle vehicle_template_;

		int choked_tentacles_;
		int multi_chokes_;

		int choke_threshold_;
		int multi_threshold_;

		float wall_proximity_threshold_;

		float attack_damage_;
		float attack_range_;

		float sense_range_;



		static void Initialize(const EnemyAIManagerInitStruct& init_struct, const Vehicle& vehicle)
		{
			if (i_ == nullptr)
			{
				i_ = new EnemyAIManager(init_struct, vehicle);
			}
		}

		static void SetPlayers(s_ptr<GameObject> player_1, s_ptr<GameObject> player_2)
		{
			assert(player_1 != nullptr);
			assert(player_2 != nullptr);
			assert(player_1 != player_2);

			player_1_ = player_1;
			player_2_ = player_1;

			players_set = true;
		}

		static void Destroy()
		{
			if (i_ != nullptr)
			{
				delete i_;
				i_ = nullptr;
			}

			player_1_ = nullptr;
			player_2_ = nullptr;
		}
	
		void UpdateEnemyStateMachine(s_ptr<ai::EnemyStateMachine> machine);
	};
}

#endif // !ENEMY_AI_MANAGER_h