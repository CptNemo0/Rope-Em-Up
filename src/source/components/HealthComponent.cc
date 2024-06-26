#include "../../headers/components/HealthComponent.h"
#include "../../headers/HealthManager.h"
#include "../../headers/ChokeList.h"
namespace components
{
	//fraction should be from 0.0 do 1.0
	//return max_health_ * fraction
	float HealthComponent::MaxHealthFraction(float fraction)
	{
		return (max_health_ * fraction);
	}

	void HealthComponent::TakeDamage(float damage)
	{
		if (damage_cooldown_)
		{
			return;
		}

		this->FroceDamage(damage);
		DamageCooldown(damage);
	}

	void HealthComponent::FroceDamage(float damage)
	{
		if (shield_ > 0.0f)
		{
			shield_ -= damage;

			if (shield_ < 0.0f)
			{
				health_ += shield_;
				shield_ = 0.0f;
			}
		}
		else
		{
			if (this->type_ == MONSTER)
			{
				auto animator = gameObject_.lock()->transform_->children_[0]->game_object_->GetComponent<components::Animator>();
				if (health_ == 1)
				{
					animator->PlayAnimation("Death_1", 2, 0.9f);
				}
				else if (health_ == 2)
				{
					animator->PlayAnimation("Death_2", 2, 0.9f);
				}
				else if (health_ == 3)
				{
					animator->PlayAnimation("Death_3", 2, 0.9f);
				}
				else if (health_ == 4)
				{
					animator->PlayAnimation("Death_4", 2, 0.9f);
				}
				else if (health_ == 5)
				{
					animator->PlayAnimation("Death_5", 2, 0.9f);
				}
			}

			health_ -= damage;
			
		}
	}

	void HealthComponent::ForceHeal(float heal)
	{
		//audio::AudioManager::i_->PlaySound(res::get_sound("res/sounds/heal.wav"));
		health_ += heal;

		if (health_ > max_health_)
		{
			health_ = max_health_;
		}


		auto emmiter = gameObject_.lock()->GetComponent<components::ParticleEmitter>();

		if (emmiter != nullptr)
		{
			glm::vec3 dir1 = glm::normalize(gameObject_.lock()->transform_->get_position() - gameObject_.lock()->transform_->get_previous_position());
			auto tex = res::get_texture("res/particles/hp.png");
			emmiter->texture_ = tex;
			emmiter->start_acceleration_ = glm::vec3(0.0f, 50.0f, 0.0f);
			emmiter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			emmiter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
			emmiter->start_size_ = {0.5f, 0.5f};
			emmiter->start_position_ = {0.0f, 1.0f, 0.0f};
			emmiter->Burst(1);
			
		}
	}

	void HealthComponent::AddShield(float shield)
	{
		shield_ += shield;
	}

	void HealthComponent::DamageCooldown(float dmg)
	{
		damage_cooldown_ = true;

		float delay = 1.0f;

		Timer::AddTimer(delay,
		[this, dmg]()
		{
			if(this != nullptr)
			{
				damage_cooldown_ = false;
				
			}
		});

		delay = 0.2f;
		Timer::AddTimer(0.2f,
		[this]()
		{
			if(this != nullptr)
			{
				if (this->type_ == PLAYER)
				{
					if (this->gameObject_.lock() != nullptr)
					{
						std::shared_ptr<components::MeshRenderer> renderer = this->gameObject_.lock()->GetComponent<components::MeshRenderer>();
						if (renderer != nullptr)
						{
							renderer->color_ = glm::vec3(1.0f);
						}
					}	
				}
			}
			
		},
		[this, delay](float delta_time)
		{
			static float t = 0.0f;
			float completion = t / delay;
			if (this->type_ == PLAYER)
			{
				if (this != nullptr)
				{
					if (this->gameObject_.lock() != nullptr)
					{
						std::shared_ptr<components::MeshRenderer> renderer = this->gameObject_.lock()->GetComponent<components::MeshRenderer>();
						if (renderer != nullptr)
						{
							renderer->color_ = glm::vec3(1.0f, 0.0f, 0.0f);
						}
					}
				}
			}

			t += delta_time;
		});

	}

	void HealthComponent::Start()
	{
	}

	void HealthComponent::Update()
	{
	}

	void HealthComponent::Destroy()
	{
		ChokeList::i_->RemoveHealthComponent(shared_from_this());
		HealthManager::i_->RemoveHealthComponent(shared_from_this());
		std::cout << "Destroying HealthCompoent" << std::endl;
	}

    HealthComponent::HealthComponent(json &j)
    {
		this->HealthComponent::HealthComponent(j["max_health"], j["health_type"]);
		health_ = j["current_health"];
		damage_cooldown_ = j["damage_cooldown"];
    }

    json HealthComponent::Serialize()
    {
        json j;

		j["health_type"] = type_;
		j["current_health"] = health_;
		j["max_health"] = max_health_;
		j["damage_cooldown"] = damage_cooldown_;

		return j;
    }
}