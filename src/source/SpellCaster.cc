#include "../headers/SpellCaster.h"

SpellCaster* SpellCaster::i_ = nullptr;

SpellCaster::SpellCaster()
{
	this->active_ = false;
	this->current_caster_ = std::vector <std::shared_ptr<components::SpellSlotComponent>>();
}

void SpellCaster::QueueUpSpell(std::shared_ptr<components::SpellSlotComponent> s)
{
	if (active_)
	{
		if (s->type_ != NOT_A_SPELL)
		{
			current_caster_.push_back(s);
			active_ = false;
		}	
	}
}

void SpellCaster::Cast()
{
	auto caster = current_caster_[0];
	auto spell = caster->type_;
	caster->spell_levels[spell]++;
	caster->type_ = NOT_A_SPELL;

	switch (spell)
	{
	case NOT_A_SPELL:
		break;
	case SKULL_MINION:
		
		for (int i = 0; i < caster->spell_levels[spell]; i++)
		{
			cout << "SKULL MINION" << endl;
		}
		
		break;
	default:
		break;
	}

	current_caster_.clear();
}
