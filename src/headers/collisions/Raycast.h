#ifndef RAYCASTS_H
#define RAYCASTS_H

#include "../Gameobject.h"
#include "Collisions.h"
#include "CollisionManager.h"
namespace collisions
{
	const float kRaycastDX = 0.01f;
	std::shared_ptr<GameObject> Raycast(glm::vec3 start, glm::vec3 dir, float distance, int layer);

	inline glm::vec3 SupportRaycast(const std::shared_ptr<ConvexHull> hull_a, const std::shared_ptr <collisions::ConvexHull> hull_b, glm::vec3 direction)
	{
		auto vertex_a = FindFarthestPointConvexHull(hull_a, direction);
		auto vertex_b = FindFarthestPointConvexHull(hull_b, -direction);

		return vertex_a - vertex_b;
	}
}




#endif