#include "../../headers/components/HpDropComponent.h"

namespace components
{
	void HpDropComponent::Start()
	{
	}

	void HpDropComponent::Update()
	{
	}

	void HpDropComponent::Destroy()
	{
		cout << "Destoying HpDropComponent" << endl;
		drop::FloatDropArgs args = drop::FloatDropArgs(value_);
		drop::HpDrop drop = drop::HpDrop(args);
		drop::DropManager::i_->AddHpDrop(drop);
	}
}


