#include "./headers/animation/AnimatorManager.h"

anim::AnimatorManager* anim::AnimatorManager::i_ = nullptr;

anim::AnimatorManager::AnimatorManager()
{
	aniamtor_components_ = std::deque<std::shared_ptr<components::Animator>>();
	enemy_attack_1 = res::get_animation("res/enemy/enemy_test.fbx", 0, "res/enemy/enemy_test.fbx");
	enemy_attack_2 = res::get_animation("res/enemy/enemy_test.fbx", 1, "res/enemy/enemy_test.fbx");
	enemy_attack_3 = res::get_animation("res/enemy/enemy_test.fbx", 2, "res/enemy/enemy_test.fbx");
	enemy_attack_4 = res::get_animation("res/enemy/enemy_test.fbx", 3, "res/enemy/enemy_test.fbx");
	enemy_attack_5 = res::get_animation("res/enemy/enemy_test.fbx", 4, "res/enemy/enemy_test.fbx");

	enemy_death_1 = res::get_animation("res/enemy/enemy_test.fbx", 5, "res/enemy/enemy_test.fbx");
	enemy_death_2 = res::get_animation("res/enemy/enemy_test.fbx", 6, "res/enemy/enemy_test.fbx");
	enemy_death_3 = res::get_animation("res/enemy/enemy_test.fbx", 7, "res/enemy/enemy_test.fbx");
	enemy_death_4 = res::get_animation("res/enemy/enemy_test.fbx", 8, "res/enemy/enemy_test.fbx");
	enemy_death_5 = res::get_animation("res/enemy/enemy_test.fbx", 9, "res/enemy/enemy_test.fbx");

	enemy_idle_1 = res::get_animation("res/enemy/enemy_test.fbx", 10, "res/enemy/enemy_test.fbx");
	enemy_idle_2 = res::get_animation("res/enemy/enemy_test.fbx", 11, "res/enemy/enemy_test.fbx");
	enemy_idle_3 = res::get_animation("res/enemy/enemy_test.fbx", 12, "res/enemy/enemy_test.fbx");
	enemy_idle_4 = res::get_animation("res/enemy/enemy_test.fbx", 13, "res/enemy/enemy_test.fbx");
	enemy_idle_5 = res::get_animation("res/enemy/enemy_test.fbx", 14, "res/enemy/enemy_test.fbx");
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

void anim::AnimatorManager::AddAnimatorComponent(s_ptr<components::Animator> animator)
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
