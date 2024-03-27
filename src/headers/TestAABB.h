#ifndef TESTAABB_H
#define TESTAABB_H

#include <vector>
#include "GameObject.h"
#include "glm/glm.hpp"
#include "Mesh.h"
#include <memory>
#include <math.h>

namespace testaabb
{
	struct AABB;

	static std::vector<std::shared_ptr<AABB>> colliders;

	struct AABB
	{
		std::shared_ptr<GameObject> game_object;
		glm::vec3 extremes = glm::vec3(0.0f);
	};
	
	glm::vec3 GetExtremePoint(std::shared_ptr<Mesh> mesh)
	{
		glm::vec3 return_value = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 a = glm::vec3(0.0f, 0.0f, 0.0f);

		float max_len = -FLT_MAX;

		for (int i = 0; i < mesh->vertices_.size(); i++)
		{
			a = glm::vec3(mesh->vertices_[i].position.x, 0.0f, mesh->vertices_[i].position.z);
			float len = glm::length(a);

			if (max_len < len)
			{
				max_len = len;
				return_value = a;
			}
		}
		return return_value;
	}

	std::shared_ptr<AABB> CreateCollider(std::shared_ptr<Mesh> mesh, std::shared_ptr<GameObject> game_object)
	{
		auto return_value = std::make_shared<AABB>();
		glm::vec3 extreme_point = GetExtremePoint(mesh);
		float distance = glm::length(extreme_point);

		return_value->game_object = game_object;
		return_value->extremes = glm::vec3(distance, 0.0f, distance);

		colliders.push_back(return_value);

		return return_value;
	}

	int TestAABBAABB(std::shared_ptr<AABB> a, std::shared_ptr<AABB> b)
	{
		if (fabsf(a->game_object->transform_->get_position().x - b->game_object->transform_->get_position().x) > (a->extremes.x + b->extremes.x)) return 0;
		if (fabsf(a->game_object->transform_->get_position().z - b->game_object->transform_->get_position().z) > (a->extremes.z + b->extremes.z)) return 0;
		return 1;
	}

}

#endif // !TESTAABB_H


