#ifndef HP_DROP_COMPONENT_H
#define HP_DROP_COMPONENT_H

#include "../components/Component.h"
#include "../drop/DropManager.h"
#include "../drop/Drop.h"
#include "../drop/DropArgs.h"

class HpDropComponent : public Component
{
public:
	float value_;

	HpDropComponent(float value) : value_(value) {}
	
	void Start() override;

	void Update() override;

	void Destroy() override;

};

#endif // !HP_DROP_COMPONENT_H
