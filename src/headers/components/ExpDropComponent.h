#ifndef EXP_DROP_COMPONENT_H
#define EXP_DROP_COMPONENT_H

#include "../components/Component.h"
#include "../drop/DropManager.h"
#include "../drop/Drop.h"
#include "../drop/DropArgs.h"

namespace components
{
	class ExpDropComponent : public Component
	{
	public:
		float value_;

		ExpDropComponent() : value_(0.0f) {}
		ExpDropComponent(float value) : value_(value) {}

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;
	};
}



#endif // !EXP_DROP_COMPONENT_H
