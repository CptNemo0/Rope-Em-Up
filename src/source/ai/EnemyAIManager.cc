#include "../../headers/ai/EnemyAIManager.h"

ai::EnemyAIManager* ai::EnemyAIManager::i_ = nullptr;

void ai::EnemyAIManager::UpdateEnemyStateMachine(std::shared_ptr<EnemyStateMachine> machine)
{
	if (machine->current_state_ != nullptr)
	{
		std::cout << machine->current_state_->Name() << std::endl;
	}
	else
	{
		std::cout << "NULL" << std::endl;
		return;
	}
	////Update facing wall:
	//glm::vec3 velocity = (machine->transfrom_->get_position() - machine->transfrom_->get_predicted_position())/pbd::kMsPerUpdate;
	//float velocity_mag_2 = glm::length2(velocity);

	//machine->facing_wall_ = (velocity_mag_2 < wall_proximity_threshold_ * wall_proximity_threshold_);

	glm::vec3 to_p_1 = player_1_->transform_->get_position() - machine->transfrom_->get_position();
	float to_p_1_mag2 = glm::length2(to_p_1);

	glm::vec3 to_p_2 = player_2_->transform_->get_position() - machine->transfrom_->get_position();
	float to_p_2_mag2 = glm::length2(to_p_2);

	if (to_p_1_mag2 < to_p_2_mag2)
	{
		if (to_p_1_mag2 < sense_range_ * sense_range_)
		{
			machine->target_player_ = player_1_;
			machine->in_sense_range_ = (to_p_1_mag2 < sense_range_ * sense_range_);
			machine->in_attack_range_ = (to_p_1_mag2 < attack_range_ * attack_range_);
		}
		machine->in_sense_range_ = (to_p_1_mag2 < sense_range_ * sense_range_);
		machine->in_attack_range_ = (to_p_1_mag2 < attack_range_ * attack_range_);
	}
	else
	{
		if (to_p_2_mag2 < sense_range_ * sense_range_)
		{
			machine->target_player_ = player_2_;
			machine->in_attack_range_ = (to_p_2_mag2 < attack_range_ * attack_range_);
		}
		machine->in_sense_range_ = (to_p_2_mag2 < sense_range_ * sense_range_);
		machine->in_attack_range_ = (to_p_2_mag2 < attack_range_ * attack_range_);
	}

	machine->pursuit_ = (!(choked_tentacles_ > choke_threshold_) && !(multi_chokes_ > multi_threshold_));
	machine->extrapolation_ = (machine->pursuit_ && ((choked_tentacles_ > choke_threshold_) && !(multi_chokes_ > multi_threshold_)));
	machine->evasive_manoeuvres_ = ((choked_tentacles_ > choke_threshold_) && (multi_chokes_ > multi_threshold_));

	
	machine->current_state_->Execute(machine.get());
}
