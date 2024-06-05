#ifndef SPELL_SLOT_COMPONENT
#define SPELL_SLOT_COMPONENT

#include "nlohmann/json.hpp"

#include <unordered_map>

#include "../res/Resources.h"
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

		std::unordered_map<SPELLS, int> spell_levels;

		SpellSlotComponent(SSC_INIT init);
		SpellSlotComponent() = default;

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;

		SpellSlotComponent(json &j);
		json Serialize() override;
	};
}

#endif // !SPELL_SLOT_COMPONENT
