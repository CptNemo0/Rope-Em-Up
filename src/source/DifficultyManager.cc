#include "../headers/DifficultyManager.h"

DifficultyManager* DifficultyManager::i_ = nullptr;

DifficultyManager::DifficultyManager()
{
}

void DifficultyManager::UpdateRoom(generation::Room* room)
{
	this->room_ = room;
}

void DifficultyManager::UpdateHealth(std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2)
{
	float current_hp_1 = player_1->GetComponent<components::HealthComponent>()->health_;
	float current_hp_2 = player_2->GetComponent<components::HealthComponent>()->health_;

	float hp_diff_1 = player_1_hp_ - current_hp_1;
	float hp_diff_2 = player_2_hp_ - current_hp_2;

	player_1_hp_ = current_hp_1;
	player_2_hp_ = current_hp_2;

	health_diff_ = hp_diff_1 + hp_diff_2;
}

void DifficultyManager::UpdateSettings(generation::RoomGenerationSettings* rg_settings)
{
	int sign = health_diff_ >= 0 ? 1 : -1;

	if (health_diff_ >= 0)
	{
		int a = random::RandInt(0, 1);

		if (a)
		{
			rg_settings->width += 1;
		}
		else
		{
			rg_settings->height += 1;
		}
	}
	else
	{
		int a = random::RandInt(0, 1);

		if (a)
		{
			rg_settings->width -= 1;
		}
		else
		{
			rg_settings->height -= 1;
		}
	}

	if (rg_settings->width < 1) rg_settings->width = 1;
	if (rg_settings->height < 1) rg_settings->height = 1;

	if (rg_settings->width > 5) rg_settings->width = 5;
	if (rg_settings->height > 5) rg_settings->height = 5;

	if (player_1_hp_ < 50.0f && player_2_hp_ < 50.0f)
	{
		rg_settings->barells = 2 + sign;
		rg_settings->enemies -= 1;
	}
	
	if (player_1_hp_ > 75.0f || player_2_hp_ > 75.0f)
	{
		rg_settings->barells--;
		if (rg_settings->barells < 1) rg_settings->barells = 1;
		rg_settings->enemies += 2;
		//cout << "AAAAAA" << endl;
	}

	if (rg_settings->width == 1 && rg_settings->height == 1)
	{
		if (rg_settings->enemies > 2) rg_settings->enemies = 2;
		rg_settings->clutter = random::RandInt(1, 3);
		rg_settings->lamps = random::RandInt(1, 3);
	}	
	else if(rg_settings->width > 1 || rg_settings->height > 1)
	{
		rg_settings->clutter = random::RandInt(2, 5);
		rg_settings->lamps = random::RandInt(4, 7);
	}
	else if (rg_settings->width > 2 && rg_settings->height > 2)
	{
		rg_settings->clutter = random::RandInt(8, 15);
		rg_settings->lamps = random::RandInt(6, 10);
	}

	if (rg_settings->enemies > 3)
	{
		rg_settings->barells = rg_settings->enemies - 3;
	}

	/*cout << "rg_settings->enemies " << rg_settings->enemies << std::endl;
	cout<<"rg_settings->lamps   " <<rg_settings->lamps <<endl;
	cout<<"rg_settings->clutter " <<rg_settings->clutter <<endl;
	cout<<"rg_settings->barells " <<rg_settings->barells <<endl;
	cout<<"rg_settings->width   " <<rg_settings->width <<endl;
	cout<<"rg_settings->height  " <<rg_settings->height <<endl;*/
}

