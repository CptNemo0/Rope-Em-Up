#include "../../headers/ai/EnemyState.h"

ai::PatrolState* ai::PatrolState::i_ = nullptr;
ai::IdleState* ai::IdleState::i_ = nullptr;
ai::OnAlertState* ai::OnAlertState::i_ = nullptr;
ai::AttackState* ai::AttackState::i_ = nullptr;
ai::PursuitState* ai::PursuitState::i_ = nullptr;
ai::ExtrapolationState* ai::ExtrapolationState::i_ = nullptr;
ai::EvasionState* ai::EvasionState::i_ = nullptr;
ai::ArrayState* ai::ArrayState::i_ = nullptr;


ai::IdleState* ai::IdleState::Instance()
{
	if (i_ == nullptr)
	{
		i_ = new IdleState();
	}
	return i_;
}

void ai::IdleState::Execute(EnemyStateMachine* machine)
{
	if (machine->rest_timer_ < machine->vehicle_.rest_lenght)
	{
		cout << "IDLE" << endl;
		if (!machine->is_choked_)
		{
			cout << "NOT CHOKED" << endl;
			cout << machine->rest_timer_ << endl;
			machine->rest_timer_ += 0.25f;
		}
		
		machine->generator_->direction_ = glm::vec3(0.0f);
		machine->generator_->magnitude_ = 0.0f;
	}
	else
	{
		machine->rest_timer_ = 0.0f;
		/*if (!machine->facing_wall_)
		{
			machine->current_state_ =
		}*/

		machine->current_state_ = PatrolState::Instance();
	}
}

ai::PatrolState* ai::PatrolState::Instance()
{
	if (i_ == nullptr)
	{
		i_ = new PatrolState();
	}
	return i_;
}

void ai::PatrolState::Execute(EnemyStateMachine* machine)
{
	if (machine->is_choked_)
	{
		machine->current_state_ = IdleState::Instance();
	}
	if (machine->in_attack_range_)
	{
		machine->current_state_ = OnAlertState::Instance();
	}
	else
	{
		if (!machine->in_sense_range_)
		{
			glm::vec3 wander_force = Wander(machine->transfrom_, machine->vehicle_, pbd::kMsPerUpdate);
			//cout << "wander force x: " << wander_force.x << " y: " << wander_force.y << " z: " << wander_force.z << endl;
			glm::vec3 wall_avoidance = WallAvoidance(machine->transfrom_, machine->vehicle_, pbd::kMsPerUpdate);
			//cout << "wall_avoidance x: " << wall_avoidance.x << " y: " << wall_avoidance.y << " z: " << wall_avoidance.z << endl;

			glm::vec3 output_force = wander_force + wall_avoidance;
			if (output_force != glm::vec3(0.0f))
			{
				output_force = glm::normalize(output_force);
			}

			machine->generator_->direction_ = output_force;
			machine->generator_->magnitude_ = machine->vehicle_.wander_speed_;
		}
		else
		{
			machine->current_state_ = OnAlertState::Instance();
		}
	}
}

ai::OnAlertState* ai::OnAlertState::Instance()
{
	if (i_ == nullptr)
	{
		i_ = new OnAlertState();
	}
	return i_;
}

void ai::OnAlertState::Execute(EnemyStateMachine* machine)
{
	if (machine->is_choked_)
	{
		machine->current_state_ = IdleState::Instance();
	}
	if (machine->another_tentacle_is_choked_)
	{
		assert(machine->current_state_ && ArrayState::Instance());
		machine->current_state_ = ArrayState::Instance();
	}
	else if(machine->pursuit_)
	{
		assert(machine->current_state_ && PursuitState::Instance());
		machine->current_state_ = PursuitState::Instance();
	}
	else if (machine->extrapolation_)
	{
		assert(machine->current_state_ && ExtrapolationState::Instance());
		machine->current_state_ = ExtrapolationState::Instance();
	}
	else if (machine->evasive_manoeuvres_)
	{
		assert(machine->current_state_ && EvasionState::Instance());
		machine->current_state_ = EvasionState::Instance();
	}
}

ai::AttackState* ai::AttackState::Instance()
{
	if (i_ == nullptr)
	{
		i_ = new AttackState();
	}
	return i_;
}

void ai::AttackState::Execute(EnemyStateMachine* machine)
{
	float tmp = 10.0f;
	if (machine->is_choked_)
	{
		machine->current_state_ = IdleState::Instance();
	}
	else
	{
		if (!machine->is_attacking)
		{
			machine->is_attacking = true;
			Timer::AddTimer(0.45f,
				[machine]
				{
					if (machine->in_attack_range_)
					{
						cout << "ATTACK!!\n";
						machine->target_player_->GetComponent<components::HealthComponent>()->TakeDamage(5.0f);
					}
					machine->is_attacking = false;
					assert(machine->current_state_ && PatrolState::Instance());
					machine->current_state_ = PatrolState::Instance();
				},
				[machine](float a)
				{
					machine->generator_->direction_ = glm::normalize(machine->target_player_->transform_->get_position() - machine->transfrom_->get_position());
					machine->generator_->magnitude_ = machine->generator_->magnitude_ * 0.85f;
				},
				false);

		}
	}
	
	
}

ai::PursuitState* ai::PursuitState::Instance()
{
	if (i_ == nullptr)
	{
		i_ = new PursuitState();
	}
	return i_;
}

void ai::PursuitState::Execute(EnemyStateMachine* machine)
{
	if (machine->is_choked_)
	{
		machine->current_state_ = IdleState::Instance();
	}
	if (machine->in_attack_range_)
	{
		assert(machine->current_state_ && AttackState::Instance());
		machine->current_state_ = AttackState::Instance();
	}
	else
	{
		if (machine->in_sense_range_)
		{
			auto t = machine->target_player_->transform_;
			glm::vec3 pursuit = Pursuit(t->get_position(), t->get_forward(), machine->transfrom_, machine->vehicle_, pbd::kMsPerUpdate);
			glm::vec3 wall_avoidance = WallAvoidance(machine->transfrom_, machine->vehicle_, pbd::kMsPerUpdate);

			glm::vec3 output_force = pursuit + wall_avoidance;
			if (output_force != glm::vec3(0.0f))
			{
				output_force = glm::normalize(output_force);
			}


			machine->generator_->direction_ = output_force;
			machine->generator_->magnitude_ = machine->vehicle_.pursuit_speed_;
		}
		else
		{
			assert(machine->current_state_ && PatrolState::Instance());
			machine->current_state_ = PatrolState::Instance();
		}
	}	
}

ai::ArrayState* ai::ArrayState::Instance()
{
	if (i_ == nullptr)
	{
		i_ = new ArrayState();
	}
	return i_;
}

void ai::ArrayState::Execute(EnemyStateMachine* machine)
{
	//to implement
}

ai::ExtrapolationState* ai::ExtrapolationState::Instance()
{
	if (i_ == nullptr)
	{
		i_ = new ExtrapolationState();
	}
	return i_;
}

void ai::ExtrapolationState::Execute(EnemyStateMachine* machine)
{
	if (machine->is_choked_)
	{
		machine->current_state_ = IdleState::Instance();
	}
	if (machine->in_attack_range_)
	{
		assert(machine->current_state_ && AttackState::Instance());
		machine->current_state_ = AttackState::Instance();
	}
	else
	{
		if (machine->in_sense_range_)
		{
			auto t = machine->target_player_->transform_;
			glm::vec3 pursuit = ExtrapolatedPursuit(t->get_position(), t->get_forward(), machine->transfrom_, machine->vehicle_, pbd::kMsPerUpdate);
			glm::vec3 wall_avoidance = WallAvoidance(machine->transfrom_, machine->vehicle_, pbd::kMsPerUpdate);

			glm::vec3 output_force = pursuit + wall_avoidance;
			if (output_force != glm::vec3(0.0f))
			{
				output_force = glm::normalize(output_force);
			}


			machine->generator_->direction_ = output_force;
			machine->generator_->magnitude_ = machine->vehicle_.pursuit_speed_;
		}
		else
		{
			assert(machine->current_state_ && PatrolState::Instance());
			machine->current_state_ = PatrolState::Instance();
		}
	}
}

ai::EvasionState* ai::EvasionState::Instance()
{
	if (i_ == nullptr)
	{
		i_ = new EvasionState();
	}
	return i_;
}

void ai::EvasionState::Execute(EnemyStateMachine* machine)
{
	if (machine->is_choked_)
	{
		machine->current_state_ = IdleState::Instance();
	}
	if (machine->in_attack_range_)
	{
		assert(machine->current_state_ && AttackState::Instance());
		machine->current_state_ = AttackState::Instance();
	}
	else
	{
		if (machine->in_sense_range_)
		{
			auto t = machine->target_player_->transform_;
			glm::vec3 pursuit = ExtrapolatedPursuit(t->get_position(), t->get_forward(), machine->transfrom_, machine->vehicle_, pbd::kMsPerUpdate);
			glm::vec3 wall_avoidance = WallAvoidance(machine->transfrom_, machine->vehicle_, pbd::kMsPerUpdate);
			//glm::vec3 evasion = Evade(); 
			//to implement
			glm::vec3 output_force = pursuit + wall_avoidance;
			if (output_force != glm::vec3(0.0f))
			{
				output_force = glm::normalize(output_force);
			}


			machine->generator_->direction_ = output_force;
			machine->generator_->magnitude_ = machine->vehicle_.pursuit_speed_;
		}
		else
		{
			assert(machine->current_state_ && PatrolState::Instance());
			machine->current_state_ = PatrolState::Instance();
		}
	}
}

ai::EnemyStateMachine::~EnemyStateMachine()
{
	cout << "Destroying EnemyStateMachine" << endl;
}

ai::EnemyStateMachine::EnemyStateMachine(s_ptr<GameObject> game_object, s_ptr<pbd::BasicGenerator> generator, const Vehicle& vehicle)
{
	current_state_ = PatrolState::Instance();

	vehicle_ = vehicle;

	target_player_ = nullptr;
	transfrom_ = game_object->transform_;
	partcile_ = game_object->GetComponent<components::PBDParticle>();
	generator_ = generator;

	rest_timer_ = 0.0f;

	is_choked_ = false;

	facing_wall_ = false;
	in_sense_range_ = false;	
	in_attack_range_ = false;

	another_tentacle_is_choked_ = false;
	pursuit_ = false;
	extrapolation_ = false;
	evasive_manoeuvres_ = false;
	is_attacking = false;
}
