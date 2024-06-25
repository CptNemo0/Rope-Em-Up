#include "../../headers/components/EnemyAIComponent.h"
#include "../../headers/ai/EnemyAIManager.h"

void components::EnemyAIComponent::CheckChoke()
{
	if (collisions::ChokeCheck(gameObject_.lock(), gPRECISION, gPRECISION * collisions::kChokePrecision, collisions::kChokeDistance))
	{
		state_machine_->is_choked_ = true;
		if (gameObject_.lock() != nullptr)
		{
			if (gameObject_.lock()->GetComponent<components::HealthComponent>() != nullptr)
			{
				ChokeList::i_->AddHealthComponent(gameObject_.lock()->GetComponent<components::HealthComponent>());
			}
		}
		
	}
	else
	{
		state_machine_->is_choked_ = false;
		if (gameObject_.lock() != nullptr)
		{
			if (gameObject_.lock()->GetComponent<components::HealthComponent>() != nullptr)
			{
				ChokeList::i_->RemoveHealthComponent(gameObject_.lock()->GetComponent<components::HealthComponent>());
			}
		}
	}
}

void components::EnemyAIComponent::Start()
{
}

void components::EnemyAIComponent::Update()
{
}

void components::EnemyAIComponent::Destroy()
{
	ai::EnemyAIManager::i_->RemoveEnemyAI(shared_from_this());
	//state_machine_->~EnemyStateMachine();
	cout << "Destroying EnemyAIComponent" << endl;

}

json components::EnemyAIComponent::Serialize()
{
	json j;

	j["is_passive"] = is_passive_;

    return j;
}
