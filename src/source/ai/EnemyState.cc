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
	if (machine == nullptr)
	{
		return;
	}
	if (machine->billboard_renderer_ != nullptr)	machine->billboard_renderer_->texture_ = res::get_texture("res/emoji/dizzy.png");
	if (machine->rest_timer_ < machine->vehicle_.rest_lenght)
	{
		//cout << "IDLE" << endl;
		if (!machine->is_choked_)
		{
			//cout << "NOT CHOKED" << endl;
			//cout << machine->rest_timer_ << endl;
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
	if (machine == nullptr)
	{
		return;
	}
	if (machine->billboard_renderer_ != nullptr)	machine->billboard_renderer_->texture_ = res::get_texture("res/emoji/face_with_monocle.png");
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
	if (machine == nullptr)
	{
		return;
	}
	if (machine->billboard_renderer_ != nullptr) machine->billboard_renderer_->texture_ = res::get_texture("res/emoji/alert.png");
	
	if (machine->was_idle_)
	{
		Shuffler<s_ptr<audio::AudioBuffer>> audio_shuffler_;
		audio_shuffler_.SetData({ res::get_sound("res/sounds/alert1.wav"), res::get_sound("res/sounds/alert2.wav"), res::get_sound("res/sounds/alert3.wav") });

		audio::AudioManager::i_->PlaySound(audio_shuffler_.Pop());
		machine->was_idle_ = false;
	}
	
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
	if (machine == nullptr)
	{
		return;
	}
	if (machine->billboard_renderer_ != nullptr) machine->billboard_renderer_->texture_ = res::get_texture("res/emoji/attack.png");

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

			// create hitbox mesh
			glm::vec3 position = machine->transfrom_->get_global_position();
			glm::vec3 forward = machine->transfrom_->get_forward();
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			float width = 1.25f;
			float length = 0.0f;

			std::shared_ptr<components::HealthComponent> hc = nullptr;

			if (machine != nullptr && machine->transfrom_ != nullptr && machine->transfrom_->game_object_ != nullptr)
			{
				hc = machine->transfrom_->game_object_->GetComponent<components::HealthComponent>();
			}

			if (hc == nullptr)
			{
				length = 1.0f;
			}
			else
			{
				length = hc->health_ * 1.5f;
			}
			
			glm::vec3 length_vec = length * forward;

			glm::vec3 lb = glm::cross(up, forward) * width;
			glm::vec3 rb = -lb;

			glm::vec3 lt = lb + length_vec;
			glm::vec3 rt = rb + length_vec;

			lb += position;
			rb += position;
			lt += position;
			rt += position;


			std::shared_ptr<GameObject> hitbox;

			if (machine != nullptr && machine->transfrom_->game_object_ != nullptr)
			{
				hitbox = GameObject::Create(machine->transfrom_->game_object_);
				hitbox->transform_->TeleportToPosition(position);
				hitbox->AddComponent(HitboxManager::i_->CreateRenderer(lb, rb, rt, lt));
				hitbox->GetComponent<components::HitboxRenderer>()->percentage_ = 1.0f;
				machine->generator_->direction_ = glm::normalize(machine->target_player_->transform_->get_position() - machine->transfrom_->get_position());
				machine->generator_->magnitude_ = 0.0f;
				machine->partcile_->controllable_ = false;
				
			}

			machine->partcile_->rotate_ = false;

			float previous_mass = machine->partcile_->mass_;
			float new_mass = 100000.0f;

			machine->partcile_->mass_ = new_mass;
			machine->partcile_->inverse_mass_ = 1.0f / new_mass;

			lb.y = 0.0f;
			rb.y = 0.0f;
			lt.y = 0.0f;
			rt.y = 0.0f;

			hitbox->AddComponent(HitboxManager::i_->CreateCollider(lb, rb, rt, lt, 10.0f));

			auto anim = machine->transfrom_->game_object_->transform_->children_[0]->game_object_->GetComponent<components::Animator>();
			auto health_component_ = machine->transfrom_->game_object_->GetComponent<components::HealthComponent>();

			if (  anim != nullptr && health_component_ != nullptr)
			{
				if (health_component_->health_ == 1 && anim->m_Animations.contains("Attack_1"))
				{
					anim->PlayAnimation("Attack_1", 3, 2.9f);
				}
				else if (health_component_->health_ == 2 && anim->m_Animations.contains("Attack_2"))
				{
					anim->PlayAnimation("Attack_2", 3, 2.9f);
				}
				else if (health_component_->health_ == 3 && anim->m_Animations.contains("Attack_3"))
				{
					anim->PlayAnimation("Attack_3", 3, 2.9);
				}
				else if (health_component_->health_ == 4 && anim->m_Animations.contains("Attack_4"))
				{
					anim->PlayAnimation("Attack_4", 3, 2.9);
				}
				else if (health_component_->health_ == 5 && anim->m_Animations.contains("Attack_5"))
				{
					anim->PlayAnimation("Attack_5", 3, 2.9);
				}
				else
				{
					anim->PlayAnimation("Idle_1", 0, 0.0f);
				}
			}

			Timer::AddTimer(0.35f,
				[hitbox]
				{

					Timer::AddTimer(0.5f,
						[hitbox]() 
						{
							if (hitbox != nullptr)
							{
								if (auto hc = hitbox->GetComponent<components::HitboxCollider>(); hc != nullptr)
								{
									HitboxManager::i_->Check(hc);
									Shuffler<s_ptr<audio::AudioBuffer>> audio_shuffler_;
									audio_shuffler_.SetData({ res::get_sound("res/sounds/obslizgly1.wav"), res::get_sound("res/sounds/obslizgly2.wav"), res::get_sound("res/sounds/obslizgly3.wav") });

									audio::AudioManager::i_->PlaySound(audio_shuffler_.Pop());
								}
							}
						});
				});
				

			int id = Timer::AddTimer(1.1f,
				[hitbox, machine, previous_mass]
				{
					if (hitbox != nullptr)
					{
						hitbox->Destroy();
						if (machine != nullptr)
						{
							machine->is_attacking = false;
							machine->partcile_->controllable_ = true;
							assert(machine->current_state_ && PatrolState::Instance());
							machine->current_state_ = PatrolState::Instance();

							if (machine->partcile_ != nullptr)
							{
								machine->partcile_->rotate_ = true;
								machine->partcile_->mass_ = previous_mass;
								machine->partcile_->inverse_mass_ = 1.0f / previous_mass;
							}
						}	
					}
				},
				[hitbox, id](float delta_time)
				{
					if (hitbox != nullptr)
					{
						auto hr = hitbox->GetComponent<components::HitboxRenderer>();
						if (hr != nullptr)
						{
							hr->percentage_ -= delta_time;
							hr->color_ = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f - hr->percentage_);
						}
					}
				});
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
	if (machine == nullptr)
	{
		return;
	}
	if (machine->billboard_renderer_ != nullptr) machine->billboard_renderer_->texture_ = res::get_texture("res/emoji/pursuit.png");
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
	if (machine == nullptr)
	{
		return;
	}
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
	if (machine == nullptr)
	{
		return;
	}
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
	if (game_object != nullptr)
	{
		if (game_object->transform_ != nullptr)
		{
			transfrom_ = game_object->transform_;
		}

		if (auto p = game_object->GetComponent<components::PBDParticle>())
		{
			partcile_ = p;
		}
	}

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
