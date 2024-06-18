#include "../../headers/components/SkullMinion.h"
#include "../../headers/SkullMinionManager.h"

void components::SkullMinion::Start()
{
}

void components::SkullMinion::Update()
{
}

void components::SkullMinion::Destroy()
{
	SkullMinionManager::i_->RemoveMinion(shared_from_this());
}
