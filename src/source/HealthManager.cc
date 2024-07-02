#include "../headers/HealthManager.h"

#include "../headers/components/ExpDropComponent.h"
#include "../headers/components/HpDropComponent.h"
#include "../headers/components/SpellSlotComponent.h"
#include "../headers/SceneManager.h"

HealthManager* HealthManager::i_ = nullptr;


HealthManager::HealthManager()
{
	health_components_ = std::deque<std::shared_ptr<components::HealthComponent>>();
	shield_model_ = res::get_model("res/shield/shield.obj");
	shield_shader_ = res::get_shader("res/shaders/Shield.vert", "res/shaders/Shield.frag");
}

std::shared_ptr<components::HealthComponent> HealthManager::CreateHealthComponent(float health, HEALTH_TYPE type)
{
	auto hc = std::make_shared<components::HealthComponent>(health, type);
	AddHealthComponent(hc);
	return hc;
}

std::shared_ptr<components::HealthComponent> HealthManager::CreateHealthComponent(json &j)
{
    auto hc = std::make_shared<components::HealthComponent>(j);
	AddHealthComponent(hc);
	return hc;
}

void HealthManager::AddHealthComponent(std::shared_ptr<components::HealthComponent> h)
{
	health_components_.push_back(h);
}

void HealthManager::RemoveHealthComponent(std::shared_ptr<components::HealthComponent> h)
{
	auto it = std::find(health_components_.begin(), health_components_.end(), h);
	if (it != health_components_.end())
	{
		health_components_.erase(it);
		cout << "ERASING HC" << endl;
	}
}

void HealthManager::DeathUpdate()
{
	something_died_ = false;
	where_ = glm::vec3(0.0f);
	what_.clear();

	for (auto& h : health_components_)
	{
		if (h == nullptr)
		{
			continue;
		}

		if (h->active_)
		{
			if (h->health_ < 0.0001)
			{
				something_died_ = true;
				where_ = h->gameObject_.lock()->transform_->get_position();
				HEALTH_TYPE what = h->type_;
				what_.insert(what);

				if (what == BARELL)
				{
					Shuffler<s_ptr<audio::AudioBuffer>> audio_shuffler_;
					audio_shuffler_.SetData({ res::get_sound("res/sounds/barrel1.wav"), res::get_sound("res/sounds/barrel2.wav"), res::get_sound("res/sounds/barrel3.wav") });

					audio::AudioManager::i_->PlaySound(audio_shuffler_.Pop());
				}

				if (what != PLAYER)
				{
					ManageDeath(h->gameObject_.lock());
					dead_.push_back(h);
				}
				else if (what == PLAYER)
				{
					auto anim = h->gameObject_.lock()->GetComponent<components::Animator>();
					anim->SetAnimation("Death", 4);
					PlayerStatsManager::i_->player_1_->GetComponent<components::PlayerController>()->active_ = false;
					PlayerStatsManager::i_->player_2_->GetComponent<components::PlayerController>()->active_ = false;
					Timer::AddTimer(1.8f, []()
					{
						SceneManager::i_->SwitchScene("game_over");
					});
					Timer::AddTimer(7.0f, []()
					{
						SceneManager::i_->SwitchScene("main_menu");
					});
				}
			}
		}
	}

	for (auto &dead : dead_)
	{
		dead->gameObject_.lock()->Destroy();
	}
	dead_.clear();
}

void HealthManager::ManageDeath(s_ptr<GameObject> go)
{
	auto hp_comp = go->GetComponent<components::HpDropComponent>();
	auto exp_comp = go->GetComponent<components::ExpDropComponent>();
	auto spell_comp = go->GetComponent<components::SpellSlotComponent>();

	if (hp_comp != nullptr)
	{
		hp_comp->DropHp();
	}
		
	if (exp_comp != nullptr)
	{
		exp_comp->DropExp();
	}
		
	if (spell_comp != nullptr)
	{
		spell_comp->DropSpell();
	}
}

void HealthManager::DrawShields()
{
	for (auto& hc : health_components_)
	{
		if (hc->shield_ > 0.0f)
		{
			glm::vec3 position = hc->gameObject_.lock()->transform_->get_global_position();
			shield_shader_->SetVec3("position", position + glm::vec3(0.0f, 1.5f, 0.0f));
			shield_shader_->SetFloat("transparency", 0.25f);
			shield_model_->Draw();
		}
	}
}
