#ifndef DROP_H
#define DROP_H

#include "../typedef.h"
#include "DropArgs.h"
#include "../GameObject.h"
#include "../components/HealthComponent.h"
#include "../components/MeshRenderer.h"
#include "../generation/Room.h"
#include "../PlayerStatsManager.h"


namespace drop
{
	class ExpDrop
	{
	public:
		FloatDropArgs args_;

		ExpDrop(FloatDropArgs args) : args_(args) {}

		void Consume();
	};

	class HpDrop
	{
	public:
		FloatDropArgs args_;

		HpDrop(FloatDropArgs args) : args_(args) {}

		void Consume(std::vector<std::shared_ptr<GameObject>> to_be_healed);
	};

	class SpellDrop
	{
	public: 
		SpellDropArgs args_;

		SpellDrop(SpellDropArgs  args) : args_(args) {}

		void Consume(generation::Room& room);
	};
}


#endif // !DROP_H
