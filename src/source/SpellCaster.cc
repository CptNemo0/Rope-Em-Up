#include "../headers/SpellCaster.h"

SpellCaster* SpellCaster::i_ = nullptr;

SpellCaster::SpellCaster()
{
	this->active_ = false;
	this->current_caster_ = std::vector <std::shared_ptr<components::SpellSlotComponent>>();
}

void SpellCaster::QueueUpSpell(std::shared_ptr<components::SpellSlotComponent> s)
{
	if (active_)
	{
		if (s->type_ != NOT_A_SPELL)
		{
			current_caster_.push_back(s);
			active_ = false;
		}	
	}
}

void SpellCaster::Cast()
{
	auto caster = current_caster_[0];
	auto spell = caster->type_;
	caster->spell_levels[spell]++;
	caster->type_ = NOT_A_SPELL;

	switch (spell)
	{
	case NOT_A_SPELL:
		break;
	case SKULL_MINION:
	{
		for (int i = 0; i < 10; i++)
		{
			auto minion = GameObject::Create(SkullMinionManager::i_->room_->minions);

			auto minion_mesh = GameObject::Create(minion);

			glm::vec3 a = PlayerStatsManager::i_->player_1_->transform_->get_position();
			glm::vec3 b = PlayerStatsManager::i_->player_2_->transform_->get_position();

			glm::vec3 center = (a + b) * 0.5f;

			float offset_x = random::RandFloat(0.0f, 1.0f) * (random::RandFloat(0.0f, 1.0f) > 0.5f ? -1.0 : 1.0);
			float offset_z = random::RandFloat(0.0f, 1.0f) * (random::RandFloat(0.0f, 1.0f) > 0.5f ? -1.0 : 1.0);

			glm::vec3 position = glm::vec3(offset_x, 0.0, offset_z) + center;

			minion->transform_->TeleportToPosition(position);

			minion_mesh->transform_->scale(glm::vec3(2.0, 2.0, 2.0));
			minion_mesh->transform_->TeleportToPosition(glm::vec3(0.0f, 1.5f, 0.0f));

			minion_mesh->AddComponent(make_shared<components::MeshRenderer>(res::get_model("res/skull/skull_f.obj"), res::get_shader("res/shaders/GBufferPass.vert", "res/shaders/GBufferPass.frag")));
			minion->AddComponent(pbd::PBDManager::i_->CreateParticle(0.25, 0.8, minion->transform_));
			minion->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::SKULL, 18, res::get_model("res/skull/skull_f.obj"), 0, minion->transform_));
			minion->AddComponent(HealthManager::i_->CreateHealthComponent(1.0f, HEALTH_TYPE::OTHER));
			minion->AddComponent(SkullMinionManager::i_->CreateSkullMinion(minion));

			minion_mesh->AddComponent(std::make_shared<components::ParticleEmitter>(500, res::get_texture("res/textures/flame_particle.png"), res::get_shader("res/shaders/Particle.vert", "res/shaders/Particle.geom", "res/shaders/Particle.frag")));
			auto emitter = minion_mesh->GetComponent<components::ParticleEmitter>();
			emitter->emission_rate_ = 0.2f;
			emitter->start_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
			emitter->end_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
			emitter->life_time_ = 1.0f;
			emitter->start_position_ = glm::vec3(0.0f, 0.0f, 0.0f);
			emitter->start_velocity_ = glm::vec3(0.0f, 2.0f, 0.0f);
			emitter->start_size_ = glm::vec2(0.4f, 0.5f);
			emitter->end_size_ = glm::vec2(0.1f, 0.2f);
			emitter->start_position_displacement_ = 1.0f;
			emitter->emit_particles_ = true;
		}
		
	}
		
		
		break;
	default:
		break;
	}

	current_caster_.clear();
}