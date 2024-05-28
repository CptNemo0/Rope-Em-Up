#include "../../headers/components/ExpDropComponent.h"

namespace components
{
	void ExpDropComponent::Start()
	{
	}

	void ExpDropComponent::Update()
	{
	}

	void ExpDropComponent::Destroy()
	{
		cout << "Destoying ExpDropComponent" << endl;
		drop::FloatDropArgs args = drop::FloatDropArgs(value_);
		drop::ExpDrop drop = drop::ExpDrop(args);
		drop::DropManager::i_->AddExpDrop(drop);
	}
}


