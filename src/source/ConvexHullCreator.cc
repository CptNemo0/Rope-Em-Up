#include "../headers/ConvexHullCreator.h"

namespace collisions
{
	glm::vec3 ConvexHullCreator::GetExtremeAlongDirection(std::shared_ptr<Mesh> mesh, glm::vec3 direction)
	{
		float maxproj = -FLT_MAX;
		int idx = 0;

        for (int i = 0; i < mesh->vertices_.size(); i++)
        {
        float proj = glm::dot(mesh->vertices_[i].position, direction);
			if (proj > maxproj) 
			{
                maxproj = proj;
				idx = i;
			}
        }
        return mesh->vertices_[idx].position;
	}

	ConvexHullCreator::ConvexHullCreator()
	{
		precision_ = 5;
		angle_ = 360 / 5;
	}

	ConvexHullCreator::ConvexHullCreator(int precision)
	{
		assert((360 % precision) == 0);
		precision_ = precision;
		angle_ = 360 / precision_;
	}

	ConvexHullCreator::~ConvexHullCreator()
	{
	}

	const int ConvexHullCreator::get_precision()
	{
		return precision_;
	}

	void ConvexHullCreator::set_precision(int value)
	{
		assert((360 % value) == 0);
		precision_ = value;
		angle_ = 360 / value;
	}
	std::shared_ptr<ConvexHull> ConvexHullCreator::CreateConvexHull(std::shared_ptr<Mesh> mesh)
	{
		auto return_value = std::make_shared<ConvexHull>();
		return_value->local_vertices_ = std::vector<glm::vec3>();

		auto start_dir_vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		float current_rotation = 0;

		for (int i = 0; i < precision_; i++)
		{
			current_rotation = angle_ * i;
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
			return_value->local_vertices_.push_back(mesh->vertices_[idx].position);
			return_value->local_vertices_.back().y = 0;
		}

		return_value->vertices_.resize(return_value->local_vertices_.size());
		return return_value;
	}
}



