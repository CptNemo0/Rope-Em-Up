#include "../../headers/components/EnemySizeManager.h"

void components::EnemySizeManager::Start()
{
	health_component_ = gameObject_.lock()->GetComponent<components::HealthComponent>();
	model_ = gameObject_.lock()->transform_->children_[0]->game_object_;
	animator = gameObject_.lock()->transform_->children_[0]->game_object_->GetComponent<components::Animator>();
	last_health_ = health_component_->health_;
	//float difference = HAX_ENEMY_HP - health_component_->health_;
	//auto pos = model_->transform_->get_position();
	//pos.y = -(difference * 0.8f);
	//model_->transform_->TeleportToPosition(pos);
	if (health_component_->health_ == 1)
	{
		animator->SetAnimation("Idle_1", 0);
	}
	else if (health_component_->health_ == 2)
	{
		animator->SetAnimation("Idle_2", 0);
	}
	else if (health_component_->health_ == 3)
	{
		animator->SetAnimation("Idle_3", 0);
	}
	else if (health_component_->health_ == 4)
	{
		animator->SetAnimation("Idle_4", 0);
	}
	else if (health_component_->health_ == 5)
	{
		animator->SetAnimation("Idle_5", 0);
	}
	else
	{
		animator->SetAnimation("Idle_1", 0);
	}
}

void components::EnemySizeManager::Update()
{
	if (last_health_ > health_component_->health_)
	{
		//last_health_ = health_component_->health_;
		//float difference = HAX_ENEMY_HP - health_component_->health_;
		//auto pos = model_->transform_->get_position();
		//pos.y = -(difference * 0.8f);
		//model_->transform_->TeleportToPosition(pos);

		if (health_component_->health_ == 1)
		{
			animator->SetAnimation("Idle_1", 0);
		}
		else if (health_component_->health_ == 2)
		{
			animator->SetAnimation("Idle_2", 0);
		}
		else if (health_component_->health_ == 3)
		{
			animator->SetAnimation("Idle_3", 0);
		}
		else if (health_component_->health_ == 4)
		{
			animator->SetAnimation("Idle_4", 0);
		}
		else if (health_component_->health_ == 5)
		{
			animator->SetAnimation("Idle_5", 0);
		}
		else
		{
			animator->SetAnimation("Idle_1", 0);
		}
	}
}

void components::EnemySizeManager::Destroy()
{
}
