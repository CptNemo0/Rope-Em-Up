#include "../../headers/components/SpellSlotComponent.h"
#include "../../headers/drop/DropManager.h"

components::SpellSlotComponent::SpellSlotComponent(SSC_INIT init)
{
	switch (init)
	{
	case components::NO_SPELL:
		type_ = SPELLS::NOT_A_SPELL;
		break;
	case components::GET_SPELL_FROM_QUEUE:
		type_ = drop::SpellDropQueue::i_->GetSpellFromQueue();
		break;
	default:
		break;
	}
}

void components::SpellSlotComponent::Start()
{
}

void components::SpellSlotComponent::Update()
{
}

void components::SpellSlotComponent::Destroy()
{
}

void components::SpellSlotComponent::DropSpell()
{
	cout << "Dropping spell" << endl;

	drop::SpellDropArgs args; 
	args.type_ = type_;

	if (type_ != SPELLS::NOT_A_SPELL)
	{
		args.position_ = gameObject_.lock()->transform_->get_position();
		args.shader = drop::SpellDropQueue::i_->shader;
		args.mesh = drop::SpellDropQueue::i_->drop_meshes[type_ - 1]; //pierwszy  w enumie jest NOT A SPELL


		drop::SpellDrop drop = drop::SpellDrop(args);

		cout << "Adding spell drop" << endl;
		drop::DropManager::i_->AddSpellDrop(drop);
	}
}

components::SpellSlotComponent::SpellSlotComponent(json &j)
{
	this->SpellSlotComponent::SpellSlotComponent();
	type_ = j["spell_type"];
}

json components::SpellSlotComponent::Serialize()
{
    json j;

	j["spell_type"] = type_;

	return j;
}
