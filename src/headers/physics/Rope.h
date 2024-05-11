#ifndef ROPE_H
#define ROPE_H

#include "glm/glm.hpp"
#include <list>
#include "PBD.h"
#include <memory>
#include "../GameObject.h"
#include "../Model.h"
#include "../components/MeshRenderer.h"

#include "../collisions/Collisions.h"
#include "../collisions/CollisionManager.h"
#include "../collisions/Collider.h"
#include "../collisions/Raycast.h"

class Rope
{
public:
	float kDistance = 0.5f;

	std::list<std::shared_ptr<GameObject>> rope_segments_;
	std::vector<pbd::RopeConstraint*> constraints_;
	
	int rope_lenght_;
	
	float segment_mass_;
	float segment_drag_;

	std::shared_ptr<GameObject> player_begin;
	std::shared_ptr<GameObject> player_end;

	Rope(glm::vec3 start, glm::vec3 dir, int rope_length, float segment_mass, float segment_drag);
	Rope(glm::vec3 start, glm::vec3 end, float segment_mass, float segment_drag, std::shared_ptr<GameObject> scene_root, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader);

	void HelperConstrainer(std::shared_ptr<components::PBDParticle> p1, std::shared_ptr<components::PBDParticle> p2);

	void CreateSegments(glm::vec3 start, glm::vec3 end, std::shared_ptr<GameObject> scene_root, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader);
	
	void AssignPlayerBegin(std::shared_ptr<GameObject> player_begin);
	void AssignPlayerEnd(std::shared_ptr<GameObject> player_end);
	int Size();

	void ApplyMass();
	void ApplyDrag();

	void AddSegment(std::shared_ptr<GameObject> scene_root, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader);

	~Rope();
};

#endif // !ROPE_H
