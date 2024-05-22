#ifndef ROPE_H
#define ROPE_H

#include "glm/glm.hpp"
#include <list>
#include "PBD.h"
#include <memory>
#include "../GameObject.h"
#include "../Model.h"
#include "../components/MeshRenderer.h"
#include "../components/PlayerController.h"

#include "../collisions/Collisions.h"
#include "../collisions/CollisionManager.h"
#include "../collisions/Collider.h"
#include "../collisions/Raycast.h"
#include "../generation/RoomGenerator.h"
#include "../ChokeList.h"

class Rope
{
public:
	float kDistance = 0.1f;

	std::deque<s_ptr<GameObject>> rope_segments_;
	std::deque<s_ptr<pbd::RopeConstraint>> rope_constraints_;
	
	int rope_lenght_;
	
	float segment_mass_;
	float segment_drag_;

	s_ptr<GameObject> player_begin_;
	s_ptr<GameObject> player_end_;
	s_ptr<components::PlayerController> player_begin_controller_;
	s_ptr<components::PlayerController> player_end_controller_;
	bool pull_cooldown_ = false;

	Rope(glm::vec3 start, glm::vec3 dir, int rope_length, float segment_mass, float segment_drag);
	Rope(glm::vec3 start, glm::vec3 end, float segment_mass, float segment_drag, std::shared_ptr<GameObject> scene_root, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader);

	void CreateSegments(glm::vec3 start, glm::vec3 end, std::shared_ptr<GameObject> scene_root, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader);
	
	void AssignPlayerBegin(std::shared_ptr<GameObject> player_begin);
	void AssignPlayerEnd(std::shared_ptr<GameObject> player_end);
	int Size();

	void ApplyMass();
	void ApplyDrag();

	void AddSegment(std::shared_ptr<GameObject> scene_root, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader);
	void RemoveSegment();

	void ChokeCheck(generation::Room *room);

	~Rope();
};

#endif // !ROPE_H
