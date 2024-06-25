#include "../headers/TutorialManager.h"

TutorialManager* TutorialManager::i_ = nullptr;

TutorialManager::TutorialManager(GLFWvidmode* mode)
{
	this->mode_ = mode;
}

void TutorialManager::Init()
{
	float aspect_ratio = Global::i_->active_camera_->get_aspect_ratio();

	int width = mode_->width;
	int height = mode_->height;
	auto movement_tutorial = GameObject::Create(SceneManager::i_->scenes_["game"]->HUD_text_root_);
	string movement_tutorial_info = "Use the left stick to move your characters.";
	movement_tutorial->AddComponent(std::make_shared<components::TextRenderer>(res::get_shader("res/shaders/HUDText.vert", "res/shaders/HUDText.frag"), res::get_font("res/fonts/CourierPrime-Regular.ttf"), movement_tutorial_info, glm::vec3(1.0f)));
	movement_tutorial->transform_->set_scale(glm::vec3(0.001f, 0.001f, 1.0f));
	movement_tutorial->transform_->scale_in({1.0f, 0.0f, 0.0f}, 1.0f / aspect_ratio);
	movement_tutorial->transform_->set_position(glm::vec3(-0.32f, -0.75f, 0.0f));
	movement_tutorial->Disable();
	tutorial_map[1] = movement_tutorial;

	auto choke_tutorial = GameObject::Create(SceneManager::i_->scenes_["game"]->HUD_text_root_);
	string choke_tutorial_info = "   Wrap your enemies with the rope,\nthen pull both triggers to choke them.";
	choke_tutorial->AddComponent(std::make_shared<components::TextRenderer>(res::get_shader("res/shaders/HUDText.vert", "res/shaders/HUDText.frag"), res::get_font("res/fonts/CourierPrime-Regular.ttf"), choke_tutorial_info, glm::vec3(1.0f)));
	choke_tutorial->transform_->set_scale(glm::vec3(0.001f, 0.001f, 1.0f));
	choke_tutorial->transform_->scale_in({1.0f, 0.0f, 0.0f}, 1.0f / aspect_ratio);
	choke_tutorial->transform_->set_position(glm::vec3(-0.28f, -0.75f, 0.0f));
	choke_tutorial->Disable();
	tutorial_map[2] = choke_tutorial;
}

void TutorialManager::Update()
{
	bool a = false;
	bool b = false;

	if (traversed_ != rlg_->rooms_traversed_)
	{
		b = true;
		traversed_ = rlg_->rooms_traversed_;

		if (tutorial_map[current_] != nullptr)
		{
			tutorial_map[current_]->Disable();
		}
	}

	if (current_ != rlg_->built_rooms_)
	{
		a = true;
		current_ = rlg_->built_rooms_;
	}

	if (a && b)
	{
		if (tutorial_map[current_] != nullptr)
		{
			tutorial_map[current_]->Enable();
		}	
	}
}
