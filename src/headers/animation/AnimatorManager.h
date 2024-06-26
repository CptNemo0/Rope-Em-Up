#pragma once

#include ".././typedef.h"
#include <headers/components/Animator.h>
namespace anim
{
	class AnimatorManager
	{
	public:
		static AnimatorManager* i_;
		std::shared_ptr<anim::Animation> enemy_attack_2;
		std::shared_ptr<anim::Animation> enemy_attack_3;
		std::shared_ptr<anim::Animation> enemy_attack_4;
		std::shared_ptr<anim::Animation> enemy_attack_5;
		std::shared_ptr<anim::Animation> enemy_attack_1;

		std::shared_ptr<anim::Animation> enemy_death_1;
		std::shared_ptr<anim::Animation> enemy_death_2;
		std::shared_ptr<anim::Animation> enemy_death_3;
		std::shared_ptr<anim::Animation> enemy_death_4;
		std::shared_ptr<anim::Animation> enemy_death_5;

		std::shared_ptr<anim::Animation> enemy_idle_1;
		std::shared_ptr<anim::Animation> enemy_idle_2;
		std::shared_ptr<anim::Animation> enemy_idle_3;
		std::shared_ptr<anim::Animation> enemy_idle_4;
		std::shared_ptr<anim::Animation> enemy_idle_5;

		std::vector<s_ptr<components::Animator>> animators_;
	private:
		AnimatorManager();
		~AnimatorManager();

	public:
		std::deque<std::shared_ptr<components::Animator>> aniamtor_components_;
		float delta_time;
		s_ptr<components::Animator> CreateAnimatorComponent();
		void AddAnimatorComponent(s_ptr<components::Animator> animator);
		void RemoveAnimatorComponent(s_ptr<components::Animator> animator);

		void Update(float dt);

		static void Initialize()
		{
			if (i_ == nullptr)
			{
				i_ = new AnimatorManager();
			}
		}

		static void Destroy()
		{
			if (i_ != nullptr)
			{
				delete i_;
				i_ = nullptr;
			}
		}

	};
}


