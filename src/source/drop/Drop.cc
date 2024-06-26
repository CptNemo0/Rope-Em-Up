#include "../../headers/drop/Drop.h"
#include "../../headers/components/SpellSlotComponent.h"
void drop::HpDrop::Consume(std::vector<std::shared_ptr<GameObject>> to_be_healed)
{
	for (auto& a : to_be_healed)
	{
		auto hc = a->GetComponent<components::HealthComponent>();
		if (hc != nullptr)
		{
			hc->ForceHeal(args_.value_);
		}
	}
}

void drop::SpellDrop::Consume(generation::Room& room)
{
	if (args_.type_ != SPELLS::NOT_A_SPELL)
	{
		auto drop = GameObject::Create(room.drops);
		drop->transform_->TeleportToPosition(args_.position_);
		drop->AddComponent(std::make_shared<components::MeshRenderer>(args_.mesh, args_.shader));

		auto ssc = std::make_shared<components::SpellSlotComponent>();
		ssc->type_ = args_.type_;
		drop->AddComponent(ssc);
	}
}

void drop::ExpDrop::Consume()
{
	PlayerStatsManager::i_->AddExp(args_.value_);
}
