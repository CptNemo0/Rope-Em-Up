#include "../../headers/drop/DropManager.h"

drop::DropManager* drop::DropManager::i_ = nullptr;

drop::DropManager::DropManager()
{
}

void drop::DropManager::AddHpDrop(HpDrop& drop)
{
	hp_stack_.push(drop);
}

void drop::DropManager::AddExpDrop(ExpDrop& drop)
{
	exp_stack_.push(drop);
}

void drop::DropManager::AddSpellDrop(SpellDrop& drop)
{
	spell_stack_.push(drop);
}

void drop::DropManager::DropExp()
{

}

void drop::DropManager::DropHp(std::vector<std::shared_ptr<GameObject>>& to_be_healed)
{
	while (!(hp_stack_.empty()))
	{
		hp_stack_.top().Consume(to_be_healed);
		hp_stack_.pop();
	}
}

void drop::DropManager::DropSpells(generation::Room& room)
{
	while (!(spell_stack_.empty()))
	{
		auto spell = spell_stack_.top();

		spell.Consume(room);

		spell_stack_.pop();
	}
}
