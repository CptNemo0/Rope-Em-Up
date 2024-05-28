#ifndef COLLISIONS_H
#define COLLISIONS_H

#define gPRECISION 18

#include <fstream>  
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../global.h"
#include "../Mesh.h"

namespace collisions
{
	const float kCollisionSoftnes = 0.25f;
	const float kChokeDistance = 1.5f;
	const float kChokePrecision = 0.85;

	struct AABB;
	struct ConvexHull;
	struct SeparationVectors;

#pragma region AABB

	struct AABB
	{
		glm::vec3 centre = glm::vec3(0.0f);
		glm::vec3 extremes = glm::vec3(0.0f);
	};

	inline const glm::vec3 FindFarthestPointAABB(s_ptr<Mesh> mesh)
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

	inline void UpdateCentre(s_ptr<AABB> aabb, glm::vec3 position)
	{
		aabb->centre = position;
	}

	inline s_ptr<AABB> CreateAABB(s_ptr<Mesh> mesh)
	{
		auto return_value = make_shared<AABB>();

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

	inline const glm::vec3 FindFarthestPointConvexHull(const s_ptr<ConvexHull> hull, const glm::vec3& direction)
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

	struct F2FStruct
	{
		glm::vec3 fp_hull_a;
		glm::vec3 fp_hull_b;
	};

	inline const F2FStruct Find2FarthestPoints(const s_ptr<ConvexHull> hull_a, const s_ptr<ConvexHull> hull_b, const glm::vec3& direction)
	{
		assert(gPRECISION == hull_a->vertices.size());
		assert(gPRECISION == hull_b->vertices.size());

		F2FStruct return_value;

		int idx_a = 0;
		int idx_b = 0;

		float maxproj_a = -FLT_MAX;
		float maxproj_b = -FLT_MAX;
		float proj = 0.0f;


		for (int i = 0; i < gPRECISION; i++)
		{
			proj = glm::dot(hull_a->vertices[i], direction);
			if (proj > maxproj_a)
			{
				maxproj_a = proj;
				idx_a = i;
			}

			proj = glm::dot(hull_b->vertices[i], -direction);
			if (proj > maxproj_b)
			{
				maxproj_b = proj;
				idx_b = i;
			}
		}

		return_value.fp_hull_a = hull_a->vertices[idx_a];
		return_value.fp_hull_b = hull_b->vertices[idx_b];

		return return_value;
	}

	inline void UpdateVertices(s_ptr<ConvexHull> hull, const glm::mat4& model_matrix)
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

	inline s_ptr<ConvexHull> CreateConvexHull(int precision, s_ptr<Mesh> mesh)
	{
		assert((360 % precision) == 0);
		int angle = 360 / precision;

		auto return_value = make_shared<ConvexHull>();
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

	inline glm::vec3 Support(const s_ptr<ConvexHull> hull_a, const s_ptr <collisions::ConvexHull> hull_b, glm::vec3 direction)
	{
		//auto vertex_a = FindFarthestPointConvexHull(hull_a, direction);
		//auto vertex_b = FindFarthestPointConvexHull(hull_b, -direction);

		auto result = Find2FarthestPoints(hull_a, hull_b, direction);

		return result.fp_hull_a - result.fp_hull_b;
	}

	inline std::vector<glm::vec3> MinkowskisDifference(const s_ptr<ConvexHull> hull_a, const s_ptr<ConvexHull> hull_b)
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

	inline bool ConvexHullCheckFaster(const s_ptr<ConvexHull> hull_a, const s_ptr<ConvexHull> hull_b)
	{
		auto minkowski = std::vector<glm::vec3>();
		auto start_dir_vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		static glm::vec3 normal(0.0f, 1.0f, 0.0f);

		int precision = fmax(hull_a->vertices.size(), hull_b->vertices.size());
		int angle = 360 / precision;
		
		int i = 0;
		float current_rotation = angle * i;

		auto rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(current_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		auto rotated_vec4 = rotation_matrix * start_dir_vec;
		auto direction = glm::vec3(rotated_vec4.x, rotated_vec4.y, rotated_vec4.z);

		minkowski.push_back(Support(hull_a, hull_b, direction));

		i++;

		for (i; i < precision; i++)
		{
			current_rotation = angle * i;
			rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(current_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			rotated_vec4 = rotation_matrix * start_dir_vec;
			direction = glm::vec3(rotated_vec4.x, rotated_vec4.y, rotated_vec4.z);

			minkowski.push_back(Support(hull_a, hull_b, direction));

			glm::vec3 edge = minkowski[i] - minkowski[i - 1];
			auto cross = glm::cross(edge, minkowski[i]); // Jezeli jest kolizja to wszystkie corss producty beda szly w kierunku (0, 1, 0)
			auto dot = glm::dot(cross, normal);

			if (dot > 0)
			{
				return false;
			}
		}

		return true;
	}

	inline bool ConvexHullCheckFaster2(const s_ptr<ConvexHull> hull_a, const s_ptr<ConvexHull> hull_b) 
	{
		static const int PRECISION = fmax(hull_a->vertices.size(), hull_b->vertices.size());
		static const float ANGLE = 360.0f / PRECISION;
		static const glm::vec3 NORMAL(0.0f, 1.0f, 0.0f);
		static std::vector<glm::vec3> DIR_VECTORS; 
		std::vector<glm::vec3> minkowski(PRECISION);

		if (DIR_VECTORS.empty()) 
		{
			for (int i = 0; i < PRECISION; ++i) 
			{
				DIR_VECTORS.push_back(glm::rotate(glm::mat4(1.0f), glm::radians(ANGLE * i), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		minkowski[0] = Support(hull_a, hull_b, DIR_VECTORS[0]);

		for (int i = 1; i < PRECISION; ++i) 
		{
			minkowski[i] = Support(hull_a, hull_b, DIR_VECTORS[i]);

			glm::vec3 edge = minkowski[i] - minkowski[i - 1];
			glm::vec3 cross = glm::cross(edge, minkowski[i]);
			if (glm::dot(cross, NORMAL) > 0) 
			{
				return false;
			}
		}

		return true;
	}

	inline void WriteDebugFIles(const std::vector<glm::vec3>& polygon, const s_ptr<collisions::ConvexHull> A, const s_ptr<collisions::ConvexHull> B)
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
	
#pragma endregion
	
#pragma region Collisions and Separation

	inline bool AABBCollisionCheck(s_ptr<AABB> a, s_ptr<AABB> b)
	{
		if (fabsf(a->centre.x - b->centre.x) > (a->extremes.x + b->extremes.x)) return false;
		if (fabsf(a->centre.z - b->centre.z) > (a->extremes.z + b->extremes.z)) return false;
		return 1;
	}

	inline bool ConvexHullCollisionCheck(const s_ptr<ConvexHull> hull_a, const s_ptr<ConvexHull> hull_b)
	{
		return InsideDifference(MinkowskisDifference(hull_a, hull_b));
	}
	
	struct SeparationVectors
	{
		glm::vec3 sep_a;
		glm::vec3 sep_b;
	};

	inline SeparationVectors GetSeparatingVector(const s_ptr<ConvexHull> hull_a, glm::vec3 a, const s_ptr<ConvexHull> hull_b, glm::vec3 b)
	{
		SeparationVectors return_value;

		if (a == b)
		{
			return_value.sep_a = glm::vec3(0.0f);
			return_value.sep_b = glm::vec3(0.0f);
			return return_value;
		}

		glm::vec3 center_diff = a - b;

		auto fp = Find2FarthestPoints(hull_a, hull_b, center_diff);

		auto vertex_a = fp.fp_hull_a;
		auto vertex_b = fp.fp_hull_b;

		auto radius_a = glm::distance(a, vertex_a);
		auto radius_b = glm::distance(b, vertex_b);
		auto distance = glm::length(center_diff);

		float mag = radius_a + radius_b - distance;

		if (mag <= 0.0f) {
			return_value.sep_a = glm::vec3(0.0f);
			return_value.sep_b = glm::vec3(0.0f);
			return return_value;
		}
		
		glm::vec3 dir = glm::vec3(0.0f);
		if (glm::length(center_diff))
		{
			dir = glm::normalize(center_diff);
		}
		else
		{
			return_value.sep_a = glm::vec3(0.0f);
			return_value.sep_b = glm::vec3(0.0f);
			return return_value;
		}
		
		
		return_value.sep_a = dir * mag * 0.5f; //zmienic na 0.1f w razie co
		return_value.sep_b = -return_value.sep_a;

		return_value.sep_a.y = 0.0f;
		return_value.sep_b.y = 0.0f;

		return return_value;	
	}

#pragma endregion
}

#endif // !COLLISIONS_H