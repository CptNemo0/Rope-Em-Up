#include "../headers/HitboxManager.h"

HitboxManager* HitboxManager::i_ = nullptr;

HitboxManager::HitboxManager()
{
	shader_ = res::get_shader("res/shaders/Hitbox.vert", "res/shaders/Hitbox.frag");
}

std::shared_ptr<components::HitboxRenderer> HitboxManager::CreateRenderer(glm::vec3 lb, glm::vec3 rb, glm::vec3 rt, glm::vec3 lt)
{
	std::shared_ptr<components::HitboxRenderer> renderer = std::make_shared<components::HitboxRenderer>(lb, rb, lt, rt);
	AddRenderer(renderer);
	return renderer;
}

void HitboxManager::AddRenderer(std::shared_ptr<components::HitboxRenderer> r)
{
	renderers_.push_back(r);
}

void HitboxManager::RemoveRenderer(std::shared_ptr<components::HitboxRenderer> r)
{
	auto it = std::find(renderers_.begin(), renderers_.end(), r);
	if (it != renderers_.end())
	{
		renderers_.erase(it);
	}
}

void HitboxManager::Draw()
{
	shader_->Use();
	for (auto& r : renderers_)
	{
		if (r->active_)
		{
			shader_->SetVec4("color_in", r->color_);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, r->texture_->id_);
			shader_->SetInt("albedo", 0);
			r->Draw();
		}
	}
}

std::shared_ptr<components::HitboxCollider> HitboxManager::CreateCollider(glm::vec3 lb, glm::vec3 rb, glm::vec3 rt, glm::vec3 lt, float dmg)
{
	glm::vec3 centre = (lb + rt) * 0.5f;
	float distance = glm::distance(centre, rt);

	auto aabb = std::make_shared<collisions::AABB>();
	aabb->centre = centre;
	aabb->extremes = glm::vec3(distance, 0.0f, distance);

	auto hull = std::make_shared<collisions::ConvexHull>();
	hull->vertices = { lb, rb, rt, lt, lb, rb, rt, lt, lb, rb, rt, lt, lb, rb, rt, lt, lb, rb };

	auto collider = std::make_shared<components::HitboxCollider>();
	collider->bp_collider_ = aabb;
	collider->np_collider_ = hull;
	collider->layer_ = collisions::LAYERS::HITBOX;
	collider->precision_ = gPRECISION;
	collider->dmg_ = dmg;

	AddCollider(collider);

	return collider;
}

void HitboxManager::AddCollider(std::shared_ptr<components::HitboxCollider> r)
{
	colliders_.push_back(r);
}

void HitboxManager::RemoveCollider(std::shared_ptr<components::HitboxCollider> r)
{
	auto it = std::find(colliders_.begin(), colliders_.end(), r);
	if (it != colliders_.end())
	{
		colliders_.erase(it);
	}
}

void HitboxManager::CheckCollisions()
{
	for (auto& hitbox : colliders_)
	{
		for (auto& collider : collisions::CollisionManager::i_->colliders_)
		{
			if (collider->active_)
			{
				if (collisions::CollisionManager::i_->LayerCheck(hitbox->layer_, collider->layer_))
				{
					if (collisions::AABBCollisionCheck(hitbox->bp_collider_, collider->bp_collider_))
					{
						if (collisions::ConvexHullCheckFaster(hitbox->np_collider_, collider->np_collider_))
						{
							if (auto hc = collider->gameObject_.lock()->GetComponent<components::HealthComponent>(); hc != nullptr)
							{
								hc->TakeDamage(hitbox->dmg_);
							}

							if (auto hdc = collider->gameObject_.lock()->GetComponent<components::HpDropComponent>(); hdc != nullptr)
							{
								hdc->damaged_by_tentacle_ = true;
							}
						}
					}
				}
			}
		}
	}
}

void HitboxManager::Check(std::shared_ptr<components::HitboxCollider> hitbox)
{
	for (auto& collider : collisions::CollisionManager::i_->colliders_)
	{
		if (collider == nullptr)
		{
			continue;
		}
		if (collider->active_)
		{
			if (collisions::CollisionManager::i_->LayerCheck(hitbox->layer_, collider->layer_))
			{
				if (collisions::AABBCollisionCheck(hitbox->bp_collider_, collider->bp_collider_))
				{
					if (collisions::ConvexHullCheckFaster(hitbox->np_collider_, collider->np_collider_))
					{
						if (auto hc = collider->gameObject_.lock()->GetComponent<components::HealthComponent>(); hc != nullptr)
						{
							hc->TakeDamage(hitbox->dmg_);
						}

						if (auto hdc = collider->gameObject_.lock()->GetComponent<components::HpDropComponent>(); hdc != nullptr)
						{
							hdc->damaged_by_tentacle_ = true;
						}

						if (auto anim = collider->gameObject_.lock()->GetComponent<components::Animator>(); anim != nullptr)
						{

							if (anim->m_Animations.contains("Damage"))
							{
								anim->PlayAnimation("Damage", 2, 1.0f);
							}
						}
						if (auto pc = collider->gameObject_.lock()->GetComponent<components::PlayerController>(); pc != nullptr)
						{
							auto sound = pc->damage_sounds_.Pop();
							Timer::AddTimer(0.1f, [sound]()
							{
								audio::AudioManager::i_->PlaySound(sound, 1.0f);
							});
						}
					}
				}
			}
		}
	}
}
