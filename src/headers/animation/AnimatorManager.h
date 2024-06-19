#pragma once

#include ".././typedef.h"
#include <headers/components/Animator.h>

class AnimatorManager
{
public:
	static AnimatorManager* i_;

	std::vector<s_ptr<components::Animator>> animators_;
private:
	AnimatorManager();
	~AnimatorManager();

public:

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

