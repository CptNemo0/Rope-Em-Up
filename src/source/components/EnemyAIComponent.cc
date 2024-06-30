#include "../../headers/components/EnemyAIComponent.h"
#include "../../headers/ai/EnemyAIManager.h"

void components::EnemyAIComponent::CheckChoke()
{
	if (collisions::ChokeCheck(gameObject_.lock(), gPRECISION, gPRECISION * collisions::kChokePrecision, collisions::kChokeDistance))
	{
		state_machine_->is_choked_ = true;
		if (gameObject_.lock() != nullptr)
		{
			if (gameObject_.lock()->GetComponent<components::HealthComponent>() != nullptr)
			{
				ChokeList::i_->AddHealthComponent(gameObject_.lock()->GetComponent<components::HealthComponent>());
			}
		}
		
	}
	else
	{
		state_machine_->is_choked_ = false;
		if (gameObject_.lock() != nullptr)
		{
			if (gameObject_.lock()->GetComponent<components::HealthComponent>() != nullptr)
			{
				ChokeList::i_->RemoveHealthComponent(gameObject_.lock()->GetComponent<components::HealthComponent>());
			}
		}
	}
}

void components::EnemyAIComponent::Start()
{
	std::vector<s_ptr<audio::AudioBuffer>> data;
	data.push_back(res::get_sound("res/sounds/slide1.wav"));
	data.push_back(res::get_sound("res/sounds/slide2.wav"));
	data.push_back(res::get_sound("res/sounds/slide3.wav"));
	slide_sounds_.SetData(data);
}

void components::EnemyAIComponent::Update()
{
	auto player_pos = PlayerStatsManager::i_->player_1_->transform_->get_global_position();
	auto player_pos2 = PlayerStatsManager::i_->player_2_->transform_->get_global_position();
	auto avg_pos = (player_pos + player_pos2) / 2.0f;

	auto pos = gameObject_.lock()->transform_->get_global_position();
	auto distance = glm::distance(avg_pos, pos);
	volume_ = glm::abs(0.3333f - glm::clamp(distance, 0.0f, 15.0f) / 45.0f);

	if (!(state_machine_->is_attacking || state_machine_->is_choked_) && !is_passive_)
	{
		if (!slide_timer_lock_)
		{
			slide_timer_lock_ = true;
			slide_timer_ = Timer::AddTimer(0.4f, [this]()
			{
				audio::AudioManager::i_->PlaySound(slide_sounds_.Pop(), volume_);
			}, nullptr, true);
		}
	}
	else
	{
		if (slide_timer_lock_)
		{
			Timer::RemoveTimer(slide_timer_);
			slide_timer_lock_ = false;
		}
	}
}

void components::EnemyAIComponent::Destroy()
{
	if (slide_timer_lock_)
	{
		Timer::RemoveTimer(slide_timer_);
		slide_timer_lock_ = false;
	}
	ai::EnemyAIManager::i_->RemoveEnemyAI(shared_from_this());
	//state_machine_->~EnemyStateMachine();
	cout << "Destroying EnemyAIComponent" << endl;

}

json components::EnemyAIComponent::Serialize()
{
	json j;

	j["is_passive"] = is_passive_;

    return j;
}
