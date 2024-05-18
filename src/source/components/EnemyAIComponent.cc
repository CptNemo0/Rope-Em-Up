#include "../../headers/components/EnemyAIComponent.h"
#include "../../headers/ai/EnemyAIManager.h"

void components::EnemyAIComponent::CheckChoke()
{
	if (collisions::ChokeCheck(gameObject_.lock(), gPRECISION, gPRECISION * collisions::kChokePrecision, collisions::kChokeDistance))
	{
		state_machine_->is_choked_ = true;
	}
	else
	{
		state_machine_->is_choked_ = false;
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