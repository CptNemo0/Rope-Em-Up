#ifndef SPELL_SLOT_COMPONENT
#define SPELL_SLOT_COMPONENT

#include "Component.h"
#include "../drop/DropManager.h"
#include "../drop/Drop.h"
#include "../drop/DropArgs.h"
#include "../drop/SpellDropQueue.h"
#include "../global.h"

namespace components
{
	enum SSC_INIT
	{
		NO_SPELL,
		GET_SPELL_FROM_QUEUE
	};

	class SpellSlotComponent : public Component
	{
	public:

		SPELLS type_;

		SpellSlotComponent(SSC_INIT init);

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;
	};
}

#endif // !SPELL_SLOT_COMPONENT
