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
	while (!(exp_stack_.empty()))
	{
		exp_stack_.top().Consume();
		exp_stack_.pop();
	}
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

void drop::DropManager::PickUpSpell(std::shared_ptr<GameObject> player)
{
	if (player == nullptr)
	{
		return;
	}

	if (room_ == nullptr)
	{
		cout << "room_" << endl;
		return;
	}

	if (room_->drops == nullptr)
	{
		cout << "room_->drops is null" << endl;
		return;
	}

	if (room_->drops->transform_ == nullptr)
	{
		cout << "room_->drops->transform_ is null" << endl;
		return;
	}

	if (!room_->drops->transform_->children_.size())
	{
		return;
	}

	std::shared_ptr<GameObject> closest;
	float min_distance = FLT_MAX;

	for (auto& transform : room_->drops->transform_->children_)
	{
		float distance = glm::distance2(player->transform_->get_global_position(), transform->get_global_position());
		if (distance < kPickUpRange2)
		{
			if (distance < min_distance)
			{
				min_distance = distance;
				closest = transform->game_object_;
			}
		}
	}

	if (closest == nullptr)
	{
		return;
	}

	auto drop_ssc = closest->GetComponent<components::SpellSlotComponent>();
	auto player_ssc = player->GetComponent<components::SpellSlotComponent>();

	if (player_ssc->type_ == NOT_A_SPELL)
	{
		player_ssc->type_ = drop_ssc->type_;
		closest->Destroy();
	}
	else
	{
		player_ssc->DropSpell();
		player_ssc->type_ = drop_ssc->type_;
		closest->Destroy();
	}
}