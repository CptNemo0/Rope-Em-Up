#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <fstream>  
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Mesh.h"

namespace collisions
{
	struct AABB;
	struct ConvexHull;

#pragma region AABB

	struct AABB
	{
		glm::vec3 centre = glm::vec3(0.0f);
		glm::vec3 extremes = glm::vec3(0.0f);
	};

	inline const glm::vec3 FindFarthestPointAABB(std::shared_ptr<Mesh> mesh)
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

	inline void UpdateCentre(std::shared_ptr<AABB> aabb, glm::vec3 position)
	{
		aabb->centre = position;
	}

	inline std::shared_ptr<AABB> CreateAABB(std::shared_ptr<Mesh> mesh)
	{
		auto return_value = std::make_shared<AABB>();

		glm::vec3 fartherst_point = FindFarthestPointAABB(mesh);
		float distance = glm::length(fartherst_point);

		return_value->extremes = glm::vec3(distance, 0.0f, distance);

		return return_value;
	}

#pragma endregion

#pragma region ConvexHull

	struct ConvexHull
	{
		std::vector<glm::vec3> local_vertices;
		std::vector<glm::vec3> vertices;
	};

	inline const glm::vec3 FindFarthestPointConvexHull(const std::shared_ptr<ConvexHull> hull, const glm::vec3& direction)
	{
		int return_value = 0;
		float maxproj = -FLT_MAX;

		for (int i = 0; i < hull->vertices.size(); i++)
		{
			float proj = glm::dot(hull->vertices[i], direction);
			if (proj > maxproj)
			{
				maxproj = proj;
				return_value = i;
			}
		}

		return hull->vertices[return_value];
	}

	inline void UpdateVertices(std::shared_ptr<ConvexHull> hull, const glm::mat4& model_matrix)
	{
		int size = hull->vertices.size();
		assert(size == hull->local_vertices.size());

		for (int i = 0; i < size; i++)
		{
			auto vertex = glm::vec4(hull->local_vertices[i].x, hull->local_vertices[i].y, hull->local_vertices[i].z, 1.0f);
			auto v4 = model_matrix * vertex;
			hull->vertices[i] = glm::vec3(v4.x, 0.0f, v4.z);
		}
	}

	inline std::shared_ptr<ConvexHull> CreateConvexHull(int precision, std::shared_ptr<Mesh> mesh)
	{
		assert((360 % precision) == 0);
		int angle = 360 / precision;

		auto return_value = std::make_shared<ConvexHull>();
		return_value->local_vertices = std::vector<glm::vec3>();

		auto start_dir_vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		float current_rotation = 0;

		for (int i = 0; i < precision; i++)
		{
			current_rotation = angle * i;
			auto rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(current_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			auto rotated_vec4 = rotation_matrix * start_dir_vec;
			auto rotated_vec3 = glm::vec3(rotated_vec4.x, rotated_vec4.y, rotated_vec4.z);

			float maxproj = -FLT_MAX;
			int idx = 0;
			for (int i = 0; i < mesh->vertices_.size(); i++)
			{
				float proj = glm::dot(mesh->vertices_[i].position, rotated_vec3);
				if (proj > maxproj)
				{
					maxproj = proj;
					idx = i;
				}
			}
			return_value->local_vertices.push_back(mesh->vertices_[idx].position);
			return_value->local_vertices.back().y = 0;
		}

		return_value->vertices.resize(return_value->local_vertices.size());
		return return_value;
	}

#pragma endregion

#pragma region Helpers

	inline glm::vec3 Support(const std::shared_ptr<ConvexHull> hull_a, const std::shared_ptr <collisions::ConvexHull> hull_b, glm::vec3 direction)
	{
		auto vertex_a = FindFarthestPointConvexHull(hull_a, direction);
		auto vertex_b = FindFarthestPointConvexHull(hull_b, -direction);

		return vertex_a - vertex_b;
	}

	inline std::vector<glm::vec3> MinkowskisDifference(const std::shared_ptr<ConvexHull> hull_a, const std::shared_ptr<ConvexHull> hull_b)
	{
		auto minkowski = std::vector<glm::vec3>();
		auto start_dir_vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		float current_rotation = 0;
		int precision = fmax(hull_a->vertices.size(), hull_b->vertices.size());
		int angle = 360 / precision;

		for (int i = 0; i < precision; i++)
		{
			current_rotation = angle * i;
			auto rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(current_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			auto rotated_vec4 = rotation_matrix * start_dir_vec;
			auto direction = glm::vec3(rotated_vec4.x, rotated_vec4.y, rotated_vec4.z);

			minkowski.push_back(Support(hull_a, hull_b, direction));
		}
		return minkowski;
	}

	inline bool InsideDifference(const std::vector<glm::vec3>& polygon)
	{
		static glm::vec3 normal(0.0f, 1.0f, 0.0f);
		int size = polygon.size();
		assert(size >= 3);

		for (int i = 0; i < size; i++)
		{
			glm::vec3 edge = polygon[(i + 1) % size] - polygon[i];


			auto cross = glm::cross(edge, polygon[i]); // Jezeli jest kolizja to wszystkie corss producty beda szly w kierunku (0, 1, 0)
			auto dot = glm::dot(cross, normal);

			if (dot > 0)
			{
				return false;
			}
		}

		return true;
	}

	inline void WriteDebugFIles(const std::vector<glm::vec3>& polygon, const std::shared_ptr<collisions::ConvexHull> A, const std::shared_ptr<collisions::ConvexHull> B)
	{
		std::ofstream polygon_file("res/logs/polygon_debug.csv");
		for (int i = 0; i < polygon.size(); i++)
		{
			polygon_file << polygon[i].x << ", " << polygon[i].z << "\n";
		}
		polygon_file.close();

		std::ofstream coll_a_file("res/logs/coll_a_debug.csv");
		for (int i = 0; i < A->vertices.size(); i++)
		{
			coll_a_file << A->vertices[i].x << ", " << A->vertices[i].z << "\n";
		}
		coll_a_file.close();

		std::ofstream coll_b_file("res/logs/coll_b_debug.csv");
		for (int i = 0; i < B->vertices.size(); i++)
		{
			coll_b_file << B->vertices[i].x << ", " << B->vertices[i].z << "\n";
		}
		coll_b_file.close();
	}

	inline bool AABBCollisionCheck(std::shared_ptr<AABB> a, std::shared_ptr<AABB> b)
	{
		if (fabsf(a->centre.x - b->centre.x) > (a->extremes.x + b->extremes.x)) return false;
		if (fabsf(a->centre.z - b->centre.z) > (a->extremes.z + b->extremes.z)) return false;
		return 1;
	}

	inline bool ConvexHullCollisionCheck(const std::shared_ptr<ConvexHull> hull_a, const std::shared_ptr<ConvexHull> hull_b)
	{
		return InsideDifference(MinkowskisDifference(hull_a, hull_b));
	}

	inline glm::vec3 GetSeparatingVector(const std::shared_ptr<ConvexHull> hull_a, const std::shared_ptr<ConvexHull> hull_b)
	{
		glm::vec3 return_value = glm::vec3(1.0f, 0.0f, 0.0f);
		int i = 0;
		int upper_bound = 128;
		while (TRUE && (i < upper_bound))
		{
			auto p = FindFarthestPointConvexHull(hull_a, return_value);
			auto q = FindFarthestPointConvexHull(hull_b, -return_value);
			if (glm::dot(p, return_value) < glm::dot(q, -return_value))
			{
				return return_value;
			}
			auto r = glm::normalize(q - p);
			return_value = return_value - (2.0f * r * glm::dot(r, return_value));
			i++;
		}

		return glm::vec3(0.0f);

	}

#pragma endregion
}

#endif // !COLLISIONS_H