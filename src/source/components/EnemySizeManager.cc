#include "../../headers/components/EnemySizeManager.h"

void components::EnemySizeManager::Start()
{
	health_component_ = gameObject_.lock()->GetComponent<components::HealthComponent>();
	animator = gameObject_.lock()->transform_->children_[0]->game_object_->GetComponent<components::Animator>();
	last_health_ = health_component_->health_;
	if (animator != nullptr) 
	{
		if (health_component_->health_ == 1)
		{
			animator->PlayAnimation("Idle_1", 2, 2.5f);
		}
		else if (health_component_->health_ == 2)
		{
			animator->PlayAnimation("Idle_2", 2, 2.5f);
		}
		else if (health_component_->health_ == 3)
		{
			animator->PlayAnimation("Idle_3", 2, 2.5f);
		}
		else if (health_component_->health_ == 4)
		{
			animator->PlayAnimation("Idle_4", 2, 2.5f);
		}
		else if (health_component_->health_ == 5)
		{
			animator->PlayAnimation("Idle_5", 2, 2.5f);
		}
		else
		{
			animator->PlayAnimation("Idle_1", 2, 2.5f);
		}
	}
	
}

void components::EnemySizeManager::Update()
{
	health_component_ = gameObject_.lock()->GetComponent<components::HealthComponent>();
	animator = gameObject_.lock()->transform_->children_[0]->game_object_->GetComponent<components::Animator>();
	last_health_ = health_component_->health_;

	if (animator != nullptr)
	{
		if (health_component_->health_ == 1)
		{
			animator->SetAnimation("Idle_1", 1);
		}
		else if (health_component_->health_ == 2)
		{
			animator->SetAnimation("Idle_2", 1);
		}
		else if (health_component_->health_ == 3)
		{
			animator->SetAnimation("Idle_3", 1);
		}
		else if (health_component_->health_ == 4)
		{
			animator->SetAnimation("Idle_4", 1);
		}
		else if (health_component_->health_ == 5)
		{
			animator->SetAnimation("Idle_5", 1);
		}
		else
		{
			animator->SetAnimation("Idle_1", 1);
		}
	}
}

void components::EnemySizeManager::Destroy()
{
}
