#include "../../headers/ai/EnemyAIManager.h"

ai::EnemyAIManager*			 ai::EnemyAIManager::i_ = nullptr;
s_ptr<GameObject>  ai::EnemyAIManager::player_1_ = nullptr;
s_ptr<GameObject>  ai::EnemyAIManager::player_2_ = nullptr;
bool						 ai::EnemyAIManager::players_set = false;

ai::EnemyAIManager::EnemyAIManager(const EnemyAIManagerInitStruct& init_struct, const Vehicle& vehicle)
{
	this->player_1_ = nullptr;
	this->player_2_ = nullptr;
	this->vehicle_template_ = vehicle;

	this->choked_tentacles_ = init_struct.choked_tentacles;
	this->multi_chokes_ = init_struct.multi_chokes;

	this->choke_threshold_ = init_struct.choke_threshold;
	this->multi_threshold_ = init_struct.multi_threshold;

	this->wall_proximity_threshold_ = init_struct.wall_proximity_threshold;

	this->attack_damage_ = init_struct.attack_damage;
	this->attack_range_ = init_struct.attack_range;

	this->sense_range_ = init_struct.sense_range;

	this->players_set = false;
}

void ai::EnemyAIManager::UpdateAI()
{
	for (auto& a : enemy_ais_)
	{
		if (a->active_)
		{
			a->CheckChoke();
			if (!a->is_passive_)
			{
				UpdateEnemyStateMachine(a->state_machine_);
			}
		}
	}
}

std::shared_ptr<components::EnemyAIComponent> ai::EnemyAIManager::CreateEnemyAI(std::shared_ptr<GameObject> game_object)
{
	auto enemy_movement_generator_1 = make_shared<pbd::BasicGenerator>();
	if (game_object != nullptr)
	{
		if (auto particle = game_object->GetComponent<components::PBDParticle>(); particle != nullptr)
		{
			pbd::PBDManager::i_->CreateFGRRecord(particle, enemy_movement_generator_1);
		}	
	}
	
	auto enemy_state_machine_1 = make_shared<ai::EnemyStateMachine>(game_object, enemy_movement_generator_1, vehicle_template_);
	auto enemy_ai_component = make_shared<components::EnemyAIComponent>(enemy_state_machine_1, false);
	enemy_ais_.push_back(enemy_ai_component);
	return enemy_ai_component;
}

std::shared_ptr<components::EnemyAIComponent> ai::EnemyAIManager::CreateEnemyAI(std::shared_ptr<GameObject> game_object, bool is_passive)
{
	auto enemy_movement_generator_1 = make_shared<pbd::BasicGenerator>();
	if (game_object != nullptr)
	{
		if (auto particle = game_object->GetComponent<components::PBDParticle>(); particle != nullptr)
		{
			pbd::PBDManager::i_->CreateFGRRecord(particle, enemy_movement_generator_1);
		}
	}
	auto enemy_state_machine_1 = make_shared<ai::EnemyStateMachine>(game_object, enemy_movement_generator_1, vehicle_template_);
	auto enemy_ai_component = make_shared<components::EnemyAIComponent>(enemy_state_machine_1, is_passive);
	enemy_ais_.push_back(enemy_ai_component);
	return enemy_ai_component;
}

s_ptr<components::EnemyAIComponent> ai::EnemyAIManager::CreateEnemyAI(s_ptr<GameObject> game_object, json &j)
{
	bool is_passive = j["is_passive"];

	auto enemy_movement_generator_1 = make_shared<pbd::BasicGenerator>();
	if (game_object != nullptr)
	{
		if (auto particle = game_object->GetComponent<components::PBDParticle>(); particle != nullptr)
		{
			pbd::PBDManager::i_->CreateFGRRecord(particle, enemy_movement_generator_1);
		}
	}
	auto enemy_state_machine_1 = make_shared<ai::EnemyStateMachine>(game_object, enemy_movement_generator_1, vehicle_template_);
	auto enemy_ai_component = make_shared<components::EnemyAIComponent>(enemy_state_machine_1, is_passive);
	enemy_ais_.push_back(enemy_ai_component);
	return enemy_ai_component;
}

void ai::EnemyAIManager::UpdateEnemyStateMachine(s_ptr<EnemyStateMachine> machine)
{
	if (!players_set)
	{
		cout << "First set player_1_ and player_2_ variables of EnemyAIManager!!!\n";
	}

	assert(players_set);

	glm::vec3 to_p_1 = player_1_->transform_->get_position() - machine->transfrom_->get_position();
	float to_p_1_mag2 = glm::length2(to_p_1);

	glm::vec3 to_p_2 = player_2_->transform_->get_position() - machine->transfrom_->get_position();
	float to_p_2_mag2 = glm::length2(to_p_2);

	float health_component = 0.0f;

	if (machine != nullptr)
	{
		if (machine->transfrom_ != nullptr)
		{
			if (machine->transfrom_->game_object_ != nullptr)
			{
				if (auto hc = machine->transfrom_->game_object_->GetComponent<components::HealthComponent>(); hc != nullptr)
				{
					health_component = hc->health_;
				}	
			}
		}
	}

	if (to_p_1_mag2 < to_p_2_mag2)
	{
		if (to_p_1_mag2 < sense_range_ * sense_range_)
		{
			machine->target_player_ = player_1_;
			machine->in_sense_range_ = (to_p_1_mag2 < sense_range_ * sense_range_);
			machine->in_attack_range_ = (to_p_1_mag2 < attack_range_ * attack_range_ * health_component * health_component);
		}
		machine->in_sense_range_ = (to_p_1_mag2 < sense_range_ * sense_range_);
		machine->in_attack_range_ = (to_p_1_mag2 < attack_range_ * attack_range_ * health_component * health_component);
	}
	else
	{
		if (to_p_2_mag2 < sense_range_ * sense_range_)
		{
			machine->target_player_ = player_2_;
			machine->in_attack_range_ = (to_p_2_mag2 < attack_range_ * attack_range_ * health_component * health_component);
		}
		machine->in_sense_range_ = (to_p_2_mag2 < sense_range_ * sense_range_);
		machine->in_attack_range_ = (to_p_2_mag2 < attack_range_ * attack_range_ * health_component * health_component);
	}

	machine->pursuit_ = (!(choked_tentacles_ > choke_threshold_) && !(multi_chokes_ > multi_threshold_));
	machine->extrapolation_ = (!machine->pursuit_ && ((choked_tentacles_ > choke_threshold_) && !(multi_chokes_ > multi_threshold_)));
	machine->evasive_manoeuvres_ = ((choked_tentacles_ > choke_threshold_) && (multi_chokes_ > multi_threshold_));


	machine->current_state_->Execute(machine.get());
}


void ai::EnemyAIManager::RemoveEnemyAI(std::shared_ptr<components::EnemyAIComponent> a)
{
	auto it = std::find(enemy_ais_.begin(), enemy_ais_.end(), a);
	if (it != enemy_ais_.end())
	{
		enemy_ais_.erase(it);
	}
}