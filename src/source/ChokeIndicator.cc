#include "../headers/ChokeIndicator.h"
ChokeIndicator* ChokeIndicator::i_ = nullptr;

ChokeIndicator::ChokeIndicator(std::shared_ptr<components::PlayerController> controller_1, std::shared_ptr<components::PlayerController> controller_2, Rope *rope)
{
	controller_1_ = controller_1;
	controller_2_ = controller_2;
	rope_ = rope;

	indicator_0_ = res::get_texture("res/indicators/blank.png");
	indicator_1_ = res::get_texture("res/indicators/left_choke.png");
	indicator_2_ = res::get_texture("res/indicators/right_choke.png");
	indicator_3_ = res::get_texture("res/indicators/choking.png");

	renderer_ = GameObject::Create(SceneManager::i_->scenes_["game"]->HUD_root_);
	renderer_->AddComponent(std::make_shared<components::HUDRenderer>(indicator_0_, res::get_shader("res/shaders/HUD.vert", "res/shaders/HUD.frag"), glm::vec4(1.0f, 1.0f, 1.0f, 0.1f)));		
}

void ChokeIndicator::Update(float delta_time)
{
	if (!choking_)
	{
		if (rope_->pull_cooldown_)
		{
			choking_ = true;
			timer_ = 0.0f;
			if (renderer_ != nullptr)
			{
				if (renderer_->GetComponent<components::HUDRenderer>() != nullptr)
				{
					renderer_->GetComponent<components::HUDRenderer>()->texture_ = indicator_3_;
				}
			}
			
		}
		else if (controller_1_->is_pulling_ && !controller_2_->is_pulling_)
		{
			if (renderer_ != nullptr)
			{
				if (renderer_->GetComponent<components::HUDRenderer>() != nullptr)
				{
					renderer_->GetComponent<components::HUDRenderer>()->texture_ = indicator_1_;
				}
			}
		}
		else if (!controller_1_->is_pulling_ && controller_2_->is_pulling_)
		{
			if (renderer_ != nullptr)
			{
				if (renderer_->GetComponent<components::HUDRenderer>() != nullptr)
				{
					renderer_->GetComponent<components::HUDRenderer>()->texture_ = indicator_2_;
				}
			}
		}
		else
		{
			if (renderer_ != nullptr)
			{
				if (renderer_->GetComponent<components::HUDRenderer>() != nullptr)
				{
					renderer_->GetComponent<components::HUDRenderer>()->texture_ = indicator_0_;
				}
			}
		}
	}
	else
	{
		timer_ += 0.5f * delta_time;
		float transparency = (0.5f - timer_);
		if (renderer_ != nullptr)
		{
			if (renderer_->GetComponent<components::HUDRenderer>() != nullptr)
			{
				renderer_->GetComponent<components::HUDRenderer>()->color_ = glm::vec4(1.0f, 1.0f, 1.0f, transparency);
			}
		}
		
		if (transparency < 0.001f)
		{
			choking_ = false;
			if (renderer_ != nullptr)
			{
				if (renderer_->GetComponent<components::HUDRenderer>() != nullptr)
				{
					renderer_->GetComponent<components::HUDRenderer>()->color_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.4f);
					renderer_->GetComponent<components::HUDRenderer>()->texture_ = indicator_0_;
				}
			}
		}
	}
}