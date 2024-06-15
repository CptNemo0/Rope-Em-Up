#ifndef ROPE_H
#define ROPE_H

#include "glm/glm.hpp"
#include <list>
#include <memory>
#include "nlohmann/json.hpp"
#include "PBD.h"

#include "../GameObject.h"
#include "../Model.h"
#include "../components/MeshRenderer.h"
#include "../components/PlayerController.h"

#include "../collisions/Collisions.h"
#include "../collisions/CollisionManager.h"
#include "../collisions/Collider.h"
#include "../collisions/Raycast.h"
#include "../generation/Room.h"
#include "../ChokeList.h"
#include "../Model.h"
#include "../Shader.h"
#include "../audio/AudioManager.h"
#include "../Shuffler.h"
#include "../typedef.h"

class Rope
{
private:
	void AudioSetup();
	Shuffler<s_ptr<audio::AudioBuffer>> audio_shuffler_;

public:
	float kDistance = 0.1f;

	std::deque<s_ptr<GameObject>> rope_segments_;
	std::deque<s_ptr<pbd::RopeConstraint>> rope_constraints_;
	
	float segment_mass_;
	float segment_drag_;

	s_ptr<GameObject> player_begin_;
	s_ptr<GameObject> player_end_;
	bool pull_cooldown_ = false;

	std::shared_ptr<Model> model_;
	std::shared_ptr<Shader> shader_;
	std::shared_ptr<GameObject> root_;

	Rope(glm::vec3 start, glm::vec3 dir, int rope_length, float segment_mass, float segment_drag);
	Rope(glm::vec3 start, glm::vec3 end, float segment_mass, float segment_drag, std::shared_ptr<GameObject> scene_root, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader);

	void CreateSegments(glm::vec3 start, glm::vec3 end, std::shared_ptr<GameObject> scene_root, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader);
	
	void AssignPlayerBegin(std::shared_ptr<GameObject> player_begin);
	void AssignPlayerEnd(std::shared_ptr<GameObject> player_end);
	int Size();

	void ApplyMass();
	void ApplyDrag();

	void AddSegment(std::shared_ptr<GameObject> scene_root);
	void RemoveSegment();

	void ChokeCheck(generation::Room *room);

	json Serialize();
	void Deserialize(json &j);

	~Rope();
};

#endif // !ROPE_H
