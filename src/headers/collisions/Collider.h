#ifndef COLLIDER_H
#define COLLIDER_H

#include "glad/glad.h"

#include <memory>
#include "nlohmann/json.hpp"

#include "../res/Resources.h"
#include "../GameObject.h"
#include "../components/Transform.h"
#include "Collisions.h"
#include "../Model.h"

#include "../typedef.h"

namespace components
{

	class Collider : public Component, public std::enable_shared_from_this<Collider>
	{
	public:
		s_ptr<components::Transform> transform_;
		s_ptr<collisions::AABB> bp_collider_;
		s_ptr<collisions::ConvexHull> np_collider_;
		int layer_, precision_, mesh_index_;
		s_ptr<Model> model_;
		float softness_ = 1.0f;

		Collider(int layer, int precision, const s_ptr<Model> model, int mesh_index, s_ptr<Transform> transform_);
		~Collider() = default;
		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;
		void UpdateColliders();
		void PredictColliders();

		Collider(json &j, s_ptr<GameObject> go);
		json Serialize() override;
	};
}

#endif // !COLLIDER_H
