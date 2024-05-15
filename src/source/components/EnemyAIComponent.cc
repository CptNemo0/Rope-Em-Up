#include "../../headers/components/EnemyAIComponent.h"
#include "../../headers/ai/EnemyAIManager.h"

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