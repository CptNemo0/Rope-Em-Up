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

	if (player_1_ != nullptr && player_2_ != nullptr)
	{
		auto p1 = player_1_->GetComponent<components::PlayerController>();
		auto p2 = player_2_->GetComponent<components::PlayerController>();
		if (p1 != nullptr)
		{
			p1->speed_ = speed_;
			p1->pull_power_ = pull_power_;
		}

		if (p2 != nullptr)
		{
			p2->speed_ = speed_;
			p2->pull_power_ = pull_power_;
		}
	}

	p_speed_ = speed_;
	p_pull_power_ = pull_power_;
	p_max_health_ = max_health_;
	p_rope_drag_ = rope_drag_;
	p_rope_weight_ = rope_weight_;
	p_segments_num_ = segments_num_;
	p_player_level_ = player_level_;
	p_rope_level_ = rope_level_;
}

void PlayerStatsManager::LevelUp()
{
	level_++;
	unspent_levels_++;
	exp_ = 0.0f;

	kLevelUpThreshold += kLevelUpThresholdRaise;

	if (player_1_ != nullptr && player_2_ != nullptr)
	{
		auto anim1 = player_1_->GetComponent<components::Animator>();
		auto anim2 = player_2_->GetComponent<components::Animator>();
		if (anim1 != nullptr)
		{
			anim1->PlayAnimation("Upgrade", 3);
		}

		if (anim2 != nullptr)
		{
			anim2->PlayAnimation("Upgrade", 3);
		}

		auto tex = res::get_texture("res/particles/levelup.png");
		auto e1 = player_1_->GetComponent<components::ParticleEmitter>();
		auto e2 = player_2_->GetComponent<components::ParticleEmitter>();

		if (e1 != nullptr && e2 != nullptr)
		{
			e1->texture_ = tex;
			e1->start_acceleration_ = glm::vec3(0.0f, 50.0f, 0.0f);
			e1->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			e1->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
			e1->start_size_ = {1.5f, 1.5f};
			e1->start_position_ = {0.0f, 1.0f, 0.0f};

			e2->texture_ = tex;
			e2->start_acceleration_ = glm::vec3(0.0f, 50.0f, 0.0f);
			e2->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			e2->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
			e2->start_size_ = {1.5f, 1.5f};
			e2->start_position_ = {0.0f, 1.0f, 0.0f};

			e1->Burst(1);
			e2->Burst(1);
		}	
	}
}

void PlayerStatsManager::AddExp(float exp)
{
	exp_ += exp;

	if (kLevelUpThreshold <= exp_)
	{
		LevelUp();
	}
	else
	{
		glm::vec3 dir1 = glm::normalize(player_1_->transform_->get_position() - player_1_->transform_->get_previous_position());
		glm::vec3 dir2 = glm::normalize(player_2_->transform_->get_position() - player_2_->transform_->get_previous_position());
		auto tex = res::get_texture("res/particles/xp.png");
		auto e1 = player_1_->GetComponent<components::ParticleEmitter>();
		auto e2 = player_2_->GetComponent<components::ParticleEmitter>();

		if (e1 != nullptr && e2 != nullptr)
		{
			e1->texture_ = tex;
			e1->start_acceleration_ = glm::vec3(0.0f, 50.0f, 0.0f);
			e1->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			e1->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
			e1->start_size_ = {0.5f, 0.5f};
			e1->start_position_ = {0.0f, 1.0f, 0.0f};

			e2->texture_ = tex;
			e2->start_acceleration_ = glm::vec3(0.0f, 50.0f, 0.0f);
			e2->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			e2->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
			e2->start_size_ = {0.5f, 0.5f};
			e2->start_position_ = {0.0f, 1.0f, 0.0f};

			e1->Burst(1);
			e2->Burst(1);
		}
	}
}

void PlayerStatsManager::LevelUpPlayer()
{
	if (unspent_levels_ > 0)
	{
		player_level_++;
		speed_ += kSpeedLevelUp;
		pull_power_ += kPullPowerLevelUp;
		max_health_ += kHealthLevelUp;
		unspent_levels_--;
	}
}

void PlayerStatsManager::LevelDownPlayer()
{
	if (player_level_ != p_player_level_)
	{
		player_level_--;
		speed_ -= kSpeedLevelUp;
		pull_power_ -= kPullPowerLevelUp;
		max_health_ -= kHealthLevelUp;
		unspent_levels_++;
	}
}

void PlayerStatsManager::LevelUpRope()
{
	if (unspent_levels_ > 0)
	{
		rope_level_++;
		rope_drag_ += kDragLevelUp;
		rope_weight_ += kWeightLevelUp;
		segments_num_ += kSegmentLevelUp;
		unspent_levels_--;
	}
}

void PlayerStatsManager::LevelDownRope()
{
	if (rope_level_ != p_rope_level_)
	{
		rope_level_--;
		rope_drag_ -= kDragLevelUp;
		rope_weight_ -= kWeightLevelUp;
		segments_num_ -= kSegmentLevelUp;
		unspent_levels_++;
	}
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