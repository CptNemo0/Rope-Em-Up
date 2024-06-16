#ifndef HP_DROP_COMPONENT_H
#define HP_DROP_COMPONENT_H

#include "nlohmann/json.hpp"

#include "../components/Component.h"
#include "../drop/DropManager.h"
#include "../drop/Drop.h"
#include "../drop/DropArgs.h"

#include "../typedef.h"

namespace components
{
	class HpDropComponent : public Component
	{
	public:
		float value_;

		HpDropComponent(float value);

		void Start() override;

		void Update() override;

		void Destroy() override;

		void DropHp();

		HpDropComponent(json &j);
		json Serialize() override;
	};
}



#endif // !HP_DROP_COMPONENT_H
