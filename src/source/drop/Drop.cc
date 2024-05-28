#include "../../headers//drop/Drop.h"

void drop::HpDrop::Consume(std::vector<std::shared_ptr<GameObject>> to_be_healed)
{
	for (auto& a : to_be_healed)
	{
		auto hc = a->GetComponent<components::HealthComponent>();
		if (hc != nullptr)
		{
			cout << "Healing for: " << args_.value_ << endl;
			hc->ForceHeal(args_.value_);
		}
	}
}
