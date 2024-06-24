#include "../../headers/components/HitboxCollider.h"
#include "../../headers/HitboxManager.h"

namespace components
{
	void HitboxCollider::Start()
	{
	}

	void HitboxCollider::Update()
	{
	}

	void HitboxCollider::Destroy()
	{
		HitboxManager::i_->RemoveCollider(shared_from_this());
	}
}


