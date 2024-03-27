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
	struct TestAABB
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

	void Init(std::shared_ptr<TestAABB> collider, std::shared_ptr<Mesh> mesh, std::shared_ptr<GameObject> game_object)
	{
		glm::vec3 extreme_point = GetExtremePoint(mesh);
		float distance = glm::length(extreme_point);
		collider->extremes = glm::vec3(distance, 0.0f, distance);
	}
	
	std::shared_ptr<TestAABB> CreateCollider(std::shared_ptr<Mesh> mesh, std::shared_ptr<GameObject> game_object)
	{
		auto return_value = std::make_shared<TestAABB>();
		glm::vec3 extreme_point = GetExtremePoint(mesh);
		float distance = glm::length(extreme_point);

		return_value->game_object = game_object;
		return_value->extremes = glm::vec3(distance, 0.0f, distance);
		return return_value;
	}
}

#endif // !TESTAABB_H


