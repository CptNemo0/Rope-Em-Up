#pragma once

#include ".././typedef.h"
#include <headers/components/Animator.h>
namespace anim
{
	class AnimatorManager
	{
	public:
		static AnimatorManager* i_;

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


