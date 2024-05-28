#include "../../headers/drop/SpellDropQueue.h"

drop::SpellDropQueue* drop::SpellDropQueue::i_ = nullptr;

namespace drop
{
	
	SpellDropQueue::SpellDropQueue()
	{

	}

	SPELLS SpellDropQueue::GetSpellFromQueue()
	{
		if (queue_.empty())
		{
			return NOT_A_SPELL;
		}
		SPELLS spell = queue_.front();
		queue_.pop();
		return spell;
	}
}