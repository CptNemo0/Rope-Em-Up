#ifndef ENEMY_STATE_H
#define ENEMY_STATE_H

#include "../SteeringBehaviors.h"
#include "EnemyStateMachine.h"

namespace ai
{
	class EnemyStateMachine;

	class EnemyState
	{
	public:
		EnemyState() = default;
		virtual ~EnemyState() {}
		
		virtual void Execute(EnemyStateMachine* machine) = 0;
		std::string Name()
		{
			return typeid(*this).name();
		}		
	};

	class PatrolState : public EnemyState
	{
	public:
		static PatrolState* i_;
		PatrolState() = default;
		~PatrolState() = default;
		static PatrolState* Instance();
		// Inherited via EnemyState
		void Execute(EnemyStateMachine* machine) override;
	};

	class OnAlertState : public EnemyState
	{
	public:
		static OnAlertState* i_;
		OnAlertState() = default;
		~OnAlertState() = default;
		static OnAlertState* Instance();
		// Inherited via EnemyState
		void Execute(EnemyStateMachine* machine) override;
	};

	class IdleState : public EnemyState
	{
	public:
		static IdleState* i_;
		IdleState() = default;
		~IdleState() = default;
		static IdleState* Instance();
		// Inherited via EnemyState
		void Execute(EnemyStateMachine* machine) override;
	};

	class AttackState : public EnemyState
	{
	public:
		static AttackState* i_;
		AttackState() = default;
		~AttackState() = default;
		static AttackState* Instance();
		void Execute(EnemyStateMachine* machine) override;
	};

	class PursuitState : public EnemyState
	{
	public:
		static PursuitState* i_;
		PursuitState() = default;
		~PursuitState() = default;
		static PursuitState* Instance();
		void Execute(EnemyStateMachine* machine) override;
	};

	class ArrayState : public EnemyState
	{
	public:
		static ArrayState* i_;
		ArrayState() = default;
		~ArrayState() = default;
		static ArrayState* Instance();
		void Execute(EnemyStateMachine* machine) override;
	};

	class ExtrapolationState : public EnemyState
	{
	public:
		static ExtrapolationState* i_;
		ExtrapolationState() = default;
		~ExtrapolationState() = default;
		static ExtrapolationState* Instance();
		void Execute(EnemyStateMachine* machine) override;
	};

	class EvasionState : public EnemyState
	{
	public:
		static EvasionState* i_;
		EvasionState() = default;
		~EvasionState() = default;
		static EvasionState* Instance();
		void Execute(EnemyStateMachine* machine) override;
	};

	class EnemyStateMachine
	{
	public:

		EnemyStateMachine(std::shared_ptr<GameObject> game_object, std::shared_ptr<pbd::BasicGenerator> generator, const Vehicle& vehicle);
		~EnemyStateMachine() = default;

		EnemyState* current_state_;

		Vehicle vehicle_;

		std::shared_ptr<components::Transform> transfrom_;
		std::shared_ptr<components::PBDParticle> partcile_;
		std::shared_ptr<pbd::BasicGenerator> generator_;

		std::shared_ptr<GameObject> target_player_;

		float rest_timer_;

		bool is_choked_;

		bool facing_wall_;
		bool in_sense_range_;
		bool in_attack_range_;

		bool another_tentacle_is_choked_;
		bool pursuit_;
		bool extrapolation_;
		bool evasive_manoeuvres_;
	};
}

#endif // !ENEMY_STATE_H
