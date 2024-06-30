#include "../../headers/physics/Rope.h"

void Rope::AudioSetup()
{
	audio_shuffler_.SetData({
		res::get_sound("res/sounds/tentaclepull1.wav"),
		res::get_sound("res/sounds/tentaclepull2.wav"),
		res::get_sound("res/sounds/tentaclepull3.wav")
	});
}

Rope::Rope(glm::vec3 start, glm::vec3 dir, int rope_length, float segment_mass, float segment_drag)
{
	AudioSetup();
}

Rope::Rope(glm::vec3 start, glm::vec3 end, float segment_mass, float segment_drag, std::shared_ptr<GameObject> scene_root, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader)
{
	this->segment_drag_ = segment_drag;
	this->segment_mass_ = segment_mass;

	CreateSegments(start, end, scene_root, model, shader);
	AudioSetup();
}

void Rope::CreateSegments(glm::vec3 start, glm::vec3 end, std::shared_ptr<GameObject> scene_root, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader)
{
	float distance = glm::distance(start, end);
	glm::vec3 player_dir = glm::normalize(end - start);

	shader_ = shader;
	model_ = model;
	root_ = scene_root;
	int i = 0;
	
	while (distance > kDistance)
	{
		auto rope_segment = GameObject::Create(scene_root);
		rope_segment->transform_->set_scale(glm::vec3(1.3f, 1.3f, 1.3f));
		rope_segment->transform_->TeleportToPosition(start + player_dir * kDistance * (float)i);
		rope_segment->AddComponent(make_shared<components::MeshRenderer>(model, shader));
		rope_segment->AddComponent(collisions::CollisionManager::i_->CreateCollider(collisions::LAYERS::ROPE, gPRECISION, model, 0, rope_segment->transform_));
		rope_segment->AddComponent(pbd::PBDManager::i_->CreateParticle(segment_mass_, segment_drag_, rope_segment->transform_, false));

		if (i > 0)
		{
			auto constraint = pbd::PBDManager::i_->CreateRopeConstraint(rope_segments_.back()->GetComponent<components::PBDParticle>(), rope_segment->GetComponent<components::PBDParticle>(), kDistance + 0.001f);
			rope_constraints_.push_back(constraint);
		}

		rope_segments_.push_back(rope_segment);

		distance -= kDistance;
		i++;
	}
}

void Rope::AssignPlayerBegin(std::shared_ptr<GameObject> player_begin)
{
	auto front_segment = rope_segments_.front();
	auto segment_position = front_segment->transform_->get_position();
	auto player_position = player_begin->transform_->get_position();

	if (glm::distance(segment_position, player_position) != 0)
	{
		auto dir = glm::normalize(player_position - segment_position);
		player_begin->transform_->TeleportToPosition(segment_position + kDistance * dir);
	}

	auto player_particle = player_begin->GetComponent<components::PBDParticle>();
	auto segment_particle = front_segment->GetComponent<components::PBDParticle>();
	auto constraint = pbd::PBDManager::i_->CreateRopeConstraint(player_particle, segment_particle, kDistance + 0.001f);
	rope_constraints_.push_front(constraint);
	player_begin_ = player_begin;
}

void Rope::AssignPlayerEnd(std::shared_ptr<GameObject> player_end)
{
	auto back_segment = rope_segments_.back();
	auto segment_position = back_segment->transform_->get_position();
	auto player_position = player_end->transform_->get_position();

	if (glm::distance(segment_position, player_position) != 0)
	{
		auto dir = glm::normalize(player_position - segment_position);
		player_end->transform_->TeleportToPosition(segment_position + kDistance * dir);
	}

	auto player_particle = player_end->GetComponent<components::PBDParticle>();
	auto segment_particle = back_segment->GetComponent<components::PBDParticle>();

	auto constraint = pbd::PBDManager::i_->CreateRopeConstraint(segment_particle, player_particle, kDistance + 0.001f);
	rope_constraints_.push_back(constraint);
	player_end_ = player_end;
}

int Rope::Size()
{
	return rope_segments_.size();
}

void Rope::ApplyMass()
{
	float inverse_mass = 1.0f / segment_mass_;
	for (auto segment : rope_segments_)
	{
		segment->GetComponent<components::PBDParticle>()->mass_ = segment_mass_;
		segment->GetComponent<components::PBDParticle>()->inverse_mass_ = inverse_mass;
	}
}

void Rope::ApplyDrag()
{
	for (auto segment : rope_segments_)
	{
		segment->GetComponent<components::PBDParticle>()->damping_factor_ = segment_drag_;
	}
}

void Rope::AddSegment(std::shared_ptr<GameObject> scene_root)
{
	auto last_constraint = rope_constraints_.back();

	auto last_segment_particle = last_constraint->p1_;
	auto player_particle = last_constraint->p2_;

	auto new_position = last_segment_particle->transform_->get_position() + (player_particle->transform_->get_position() - last_segment_particle->transform_->get_position()) * 0.5f;

	auto rope_segment = GameObject::Create(scene_root);
	rope_segment->transform_->set_scale(glm::vec3(1.3f, 1.3f, 1.3f));
	rope_segment->transform_->TeleportToPosition(new_position);
	rope_segment->AddComponent(make_shared<components::MeshRenderer>(model_, shader_));
	rope_segment->AddComponent(collisions::CollisionManager::i_->CreateCollider(2, gPRECISION, model_, 0, rope_segment->transform_));
	rope_segment->AddComponent(pbd::PBDManager::i_->CreateParticle(segment_mass_, segment_drag_, rope_segment->transform_, false));
	rope_segments_.push_back(rope_segment);

	auto new_particle = rope_segment->GetComponent<components::PBDParticle>();

	last_constraint->p2_ = new_particle;

	auto constraint = pbd::PBDManager::i_->CreateRopeConstraint(new_particle, player_particle, kDistance + 0.001f);
	rope_constraints_.push_back(constraint);
}

void Rope::RemoveSegment()
{
	int n = rope_constraints_.size();
	
	if (n >= 2)
	{
		auto player_constraint = rope_constraints_.back();
		rope_constraints_.pop_back();
		auto plast_constraint = rope_constraints_.back();
		rope_constraints_.pop_back();

		auto plast_particle = plast_constraint->p1_;
		auto last_particle = plast_constraint->p2_;
		auto player_particle = player_constraint->p2_;

		auto last_particle_go = rope_segments_.back();
		rope_segments_.pop_back();
		last_particle_go->Destroy();

		rope_constraints_.push_back(pbd::PBDManager::i_->CreateRopeConstraint(plast_particle, player_particle, kDistance));

		// TODO:
		// Fix particle assigning cus they're still connected to the player
		// even after deleting a segment

		// last_particle_go->RemoveComponent<components::MeshRenderer>();

		//last->~RopeConstraint();
	}
}

void Rope::ChokeCheck(generation::Room *room)
{
	s_ptr<components::PlayerController> player_begin_controller_ = player_begin_->GetComponent<components::PlayerController>();
	s_ptr<components::PlayerController> player_end_controller_ = player_end_->GetComponent<components::PlayerController>();

	if (!pull_cooldown_ && player_begin_controller_->is_pulling_ && player_end_controller_->is_pulling_ && player_begin_controller_ != nullptr && player_end_controller_ != nullptr)
	{
		player_begin_->GetComponent<components::PBDParticle>()->rotation_offset_ = 180.0f;
		player_end_->GetComponent<components::PBDParticle>()->rotation_offset_ = 180.0f;

		pull_cooldown_ = true;

		player_begin_->GetComponent<components::PlayerController>()->PullRope();
		player_end_->GetComponent<components::PlayerController>()->PullRope();

		auto choked = ChokeList::i_->Choke(1.0f);
		if (choked.contains(HEALTH_TYPE::MONSTER))
		{
			cout << "CHOKE'EM MOTHAFUCKA!!!!\n";
			audio::AudioManager::i_->PlaySound(audio_shuffler_.Pop(), 0.6f);

			///dealing dmg to enemy animation to implement
		}

		Timer::AddTimer(0.1f, [this]()
		{
			player_begin_->GetComponent<components::Animator>()->PlayAnimation("Pull", 3);
			player_end_->GetComponent<components::Animator>()->PlayAnimation("Pull", 3);
		});

		Timer::AddTimer(0.5f, [this]()
		{
			player_begin_->GetComponent<components::PBDParticle>()->rotation_offset_ = 0.0f;
			player_end_->GetComponent<components::PBDParticle>()->rotation_offset_ = 0.0f;
		});

		Timer::AddTimer(1.0f, [this]()
		{
			pull_cooldown_ = false;
		});
	}
}

json Rope::Serialize()
{
    json j;

	j["rope_length"] = rope_segments_.size();
	for (auto &segment : rope_segments_)
	{
		auto pos = segment->transform_->get_position();
		j["rope_segment_positions"].push_back({pos.x, pos.y, pos.z});
	}

	j["segment_mass"] = segment_mass_;
	j["segment_drag"] = segment_drag_;

	return j;
}

void Rope::Deserialize(json &j)
{
	while (rope_constraints_.size() != 1)
	{
		RemoveSegment();
	}
	for (auto j_pos : j["rope_segment_positions"])
	{
		AddSegment(root_);
		glm::vec3 segment_pos = {j_pos[0], j_pos[1], j_pos[2]};
		rope_segments_.back()->transform_->TeleportToPosition(segment_pos);
	}
	segment_mass_ = j["segment_mass"];
	segment_drag_ = j["segment_drag"];
}

Rope::~Rope()
{
	rope_segments_.clear();
	rope_constraints_.clear();
}
