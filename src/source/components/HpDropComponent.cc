#include "../../headers/components/HpDropComponent.h"

namespace components
{
    HpDropComponent::HpDropComponent(float value)
		: value_(value)
    {
	}

    void HpDropComponent::Start()
	{
	}

	void HpDropComponent::Update()
	{
	}

	void HpDropComponent::Destroy()
	{
	}

    void HpDropComponent::DropHp()
    {
		cout << "Dropping HP" << endl;
		drop::FloatDropArgs args = drop::FloatDropArgs(value_);
		drop::HpDrop drop = drop::HpDrop(args);
		drop::DropManager::i_->AddHpDrop(drop);
    }

    HpDropComponent::HpDropComponent(json &j)
    {
		float value = j["value"];
		this->HpDropComponent::HpDropComponent(value);
    }

    json HpDropComponent::Serialize()
    {
        json j;

		j["value"] = value_;

		return j;
    }
}
