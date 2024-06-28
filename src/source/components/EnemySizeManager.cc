#include "../../headers/components/EnemySizeManager.h"

void components::EnemySizeManager::Start()
{
	health_component_ = gameObject_.lock()->GetComponent<components::HealthComponent>();
	animator = gameObject_.lock()->transform_->children_[0]->game_object_->GetComponent<components::Animator>();
}

void components::EnemySizeManager::Update()
{
	if (animator != nullptr)
	{
		auto anim_name = "Idle_" + std::to_string((int)health_component_->health_);
		if (animator->m_Animations.contains(anim_name))
		{
			animator->SetAnimation(anim_name, 1);
		}
	}
}

void components::EnemySizeManager::Destroy()
{
}
