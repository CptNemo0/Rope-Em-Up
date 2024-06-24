#ifndef HITBOX_COLLIDER_H
#define HITBOX_COLLIDER_H

#include "../collisions/Collisions.h"
#include "Component.h"
#include "../GameObject.h"
#include <memory>
#include <unordered_set>
namespace components
{
	class HitboxCollider : public Component, public std::enable_shared_from_this<HitboxCollider>
	{
	public:
		std::shared_ptr<collisions::AABB> bp_collider_;
		std::shared_ptr<collisions::ConvexHull> np_collider_;
		int layer_;
		int precision_;
		float dmg_;

		HitboxCollider() = default;

		// Inherited via Component
		void Start() override;
		void Update() override;
		void Destroy() override;
	};
}



#endif // !HITBOX_COLLIDER_H
