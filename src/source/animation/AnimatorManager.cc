#include "./headers/animation/AnimatorManager.h"

anim::AnimatorManager* anim::AnimatorManager::i_ = nullptr;

anim::AnimatorManager::AnimatorManager()
{
	aniamtor_components_ = std::deque<std::shared_ptr<components::Animator>>();
}

anim::AnimatorManager::~AnimatorManager()
{
}

s_ptr<components::Animator> anim::AnimatorManager::CreateAnimatorComponent()
{
	auto anim = std::make_shared<components::Animator>();
	AddAnimatorComponent(anim);
	return anim;
}

s_ptr<components::Animator> anim::AnimatorManager::AddAnimatorComponent(s_ptr<components::Animator> animator)
{
	aniamtor_components_.push_back(animator);
	
}

void anim::AnimatorManager::RemoveAnimatorComponent(s_ptr<components::Animator> animator)
{
	auto a = std::find(aniamtor_components_.begin(), aniamtor_components_.end(), animator);
	if (a != aniamtor_components_.end())
	{
		aniamtor_components_.erase(a);

	}
}

void anim::AnimatorManager::Update(float dt)
{
	delta_time = dt;
	for (auto& a : aniamtor_components_)
	{
		a->SetDeltaTime(dt);
	}
}
