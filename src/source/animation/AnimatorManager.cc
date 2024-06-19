#include "./headers/animation/AnimatorManager.h"

AnimatorManager::AnimatorManager()
{
	aniamtor_components_ = std::deque<std::shared_ptr<components::Animator>>();
}

AnimatorManager::~AnimatorManager()
{
}

s_ptr<components::Animator> AnimatorManager::CreateAnimatorComponent()
{
	auto anim = std::make_shared<components::Animator>();

}

s_ptr<components::Animator> AnimatorManager::AddAnimatorComponent(s_ptr<components::Animator> animator)
{
	aniamtor_components_.push_back(animator);
}

s_ptr<components::Animator> AnimatorManager::RemoveAnimatorComponent(s_ptr<components::Animator> animator)
{
	auto a = std::find(aniamtor_components_.begin(), aniamtor_components_.end(), animator);
	if (a != aniamtor_components_.end())
	{
		aniamtor_components_.erase(a);

	}
}

void AnimatorManager::Update(float dt)
{
	delta_time = dt;
	for (auto& a : aniamtor_components_)
	{
		a->SetDeltaTime(dt);
	}
}
