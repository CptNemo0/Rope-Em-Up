#include "../headers/PlayerStatsManager.h"

PlayerStatsManager* PlayerStatsManager::i_ = nullptr;

PlayerStatsManager::PlayerStatsManager(Rope* rope, std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2)
{
	this->rope_ = rope;
	player_1_ = player_1;
	player_2_ = player_2;

	level_ = 0;
	exp_ = 0.0f;
	unspent_levels_ = 0;

	speed_ = player_1->GetComponent<components::PlayerController>()->speed_;
	pull_power_ = player_1->GetComponent<components::PlayerController>()->pull_power_;
	max_health_ = player_1->GetComponent<components::HealthComponent>()->max_health_;
	rope_drag_ = rope_->segment_drag_;
	rope_weight_ = rope_->segment_mass_;
	segments_num_ = rope_->rope_segments_.size();

	p_speed_ = speed_;
	p_pull_power_ = pull_power_;
	p_max_health_ = max_health_;
	p_rope_drag_ = rope_drag_;
	p_rope_weight_ = rope_weight_;
	p_segments_num_ = segments_num_;
}

void PlayerStatsManager::Apply()
{
	rope_->segment_drag_ = rope_drag_;
	rope_->segment_mass_ = rope_weight_;
	rope_->ApplyDrag();
	rope_->ApplyMass();

	int segment_diff = (int)(rope_->rope_segments_.size()) - segments_num_;

	if (-segment_diff > 0)
	{
		for (int i = 0; i < -segment_diff; i++)
		{
			rope_->AddSegment(rope_->root_);
		}
	}

	else if (-segment_diff < 0)
	{
		for (int i = 0; i < -segment_diff; i++)
		{
			rope_->RemoveSegment();
		}
	}

	player_1_->GetComponent<components::PlayerController>()->speed_ = speed_;
	player_1_->GetComponent<components::PlayerController>()->pull_power_ = pull_power_;
	player_2_->GetComponent<components::PlayerController>()->speed_ = speed_;
	player_2_->GetComponent<components::PlayerController>()->pull_power_ = pull_power_;

	p_speed_ = speed_;
	p_pull_power_ = pull_power_;
	p_max_health_ = max_health_;
	p_rope_drag_ = rope_drag_;
	p_rope_weight_ = rope_weight_;
	p_segments_num_ = segments_num_;
}

void PlayerStatsManager::LevelUp()
{
	level_++;
	unspent_levels_++;
	exp_ = 0.0f;
	player_1_->GetComponent<components::HealthComponent>()->health_ = max_health_;
	player_2_->GetComponent<components::HealthComponent>()->health_ = max_health_;
	player_1_->GetComponent<components::Animator>()->PlayAnimation("Upgrade");
	player_2_->GetComponent<components::Animator>()->PlayAnimation("Upgrade");
}

void PlayerStatsManager::AddExp(float exp)
{
	exp_ += exp;

	if (kLevelUpThreshold <= exp_)
	{
		LevelUp();
	}


	glm::vec3 dir1 = glm::normalize(player_1_->transform_->get_position() - player_1_->transform_->get_previous_position());
	glm::vec3 dir2 = glm::normalize(player_2_->transform_->get_position() - player_2_->transform_->get_previous_position());
	auto tex = res::get_texture("res/textures/test_smoke.png");
	auto e1 = player_1_->GetComponent<components::ParticleEmitter>();
	auto e2 = player_2_->GetComponent<components::ParticleEmitter>();
	e1->texture_ = tex;
	e1->start_acceleration_ = glm::vec3(0.0f, 100.0f, 0.0f) + dir1;
	e2->texture_ = tex;
	e2->start_acceleration_ = glm::vec3(0.0f, 100.0f, 0.0f) + dir2;
	e1->Burst(10);
	e2->Burst(10);
}

void PlayerStatsManager::LevelUpSpeed()
{
	if (unspent_levels_ > 0)
	{
		speed_ += kSpeedLevelUp;
		unspent_levels_--;
	}
	
}

void PlayerStatsManager::LevelDownSpeed()
{
	if (speed_ != p_speed_)
	{
		speed_ -= kSpeedLevelUp;
		unspent_levels_++;
	}
}

void PlayerStatsManager::LevelUpPull()
{
	if (unspent_levels_ > 0)
	{
		pull_power_ += kPullPowerLevelUp;
		unspent_levels_--;
	}
}

void PlayerStatsManager::LevelDownPull()
{
	if (pull_power_ != p_pull_power_)
	{
		pull_power_ -= kPullPowerLevelUp;
		unspent_levels_++;
	}
}

void PlayerStatsManager::LevelUpDrag()
{
	if (unspent_levels_ > 0)
	{
		rope_drag_ += kDragLevelUp;
		unspent_levels_--;
	}
}

void PlayerStatsManager::LevelDownDrag()
{
	if (rope_drag_ != p_rope_drag_)
	{
		rope_drag_ -= kDragLevelUp;
		unspent_levels_++;
	}
}

void PlayerStatsManager::LevelUpWeight()
{
	if (unspent_levels_ > 0)
	{
		rope_weight_ += kWeightLevelUp;
		unspent_levels_--;
	}
}

void PlayerStatsManager::LevelDownWeight()
{
	if(rope_weight_ != p_rope_weight_)
	{
		rope_weight_ -= kWeightLevelUp;
		unspent_levels_++;
	}
}

void PlayerStatsManager::LevelUpSegments()
{
	if (unspent_levels_ > 0)
	{
		segments_num_ += kSegmentLevelUp;
		unspent_levels_--;
	}
}

void PlayerStatsManager::LevelDownSegments()
{
	if (segments_num_ != p_segments_num_)
	{
		segments_num_ -= kSegmentLevelUp;
		unspent_levels_++;
	}
}

void PlayerStatsManager::LevelUpHealth()
{
	if (unspent_levels_ > 0)
	{
		max_health_ += kHealthLevelUp;
		unspent_levels_--;
	}
}

void PlayerStatsManager::LevelDownHealth()
{
	if (max_health_ == p_max_health_)
	{
		max_health_ -= kHealthLevelUp;
		unspent_levels_++;
	}
}