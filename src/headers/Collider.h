#ifndef COLLIDER_H
#define COLLIDER_H

#include "glad/glad.h"

#include <memory>

#include "Component.h"
#include "Transform.h"
#include "Collisions.h"
#include "Mesh.h"

namespace Components
{
	class GameObject;
	

	class Collider : public Component, public std::enable_shared_from_this<Collider>
	{
	public:
		std::shared_ptr<Components::Transform> transform_;
		std::shared_ptr<collisions::AABB> bp_collider_;
		std::shared_ptr<collisions::ConvexHull> np_collider_;
		int layer_;

		Collider(int layer, int precision, const std::shared_ptr<Mesh> mesh, std::shared_ptr<Transform> transform_);
		~Collider() = default;
		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;
		void UpdateColliders();
		void PredictColliders();
	};
}

#endif // !COLLIDER_H
