#include "../../headers/components/EnemySizeManager.h"

void components::EnemySizeManager::Start()
{
	health_component_ = gameObject_.lock()->GetComponent<components::HealthComponent>();
	model_ = gameObject_.lock()->transform_->children_[0]->game_object_;
	last_health_ = health_component_->health_;
	float difference = HAX_ENEMY_HP - health_component_->health_;
	auto pos = model_->transform_->get_position();
	pos.y = -(difference * 0.8f);
	model_->transform_->TeleportToPosition(pos);
}

void components::EnemySizeManager::Update()
{
	if (last_health_ > health_component_->health_)
	{
		last_health_ = health_component_->health_;
		float difference = HAX_ENEMY_HP - health_component_->health_;
		auto pos = model_->transform_->get_position();
		pos.y = -(difference * 0.8f);
		model_->transform_->TeleportToPosition(pos);
	}
}

void components::EnemySizeManager::Destroy()
{
}
