#include "../headers/ConvexHull.h"

namespace collisions
{
	void ConvexHull::UpdateVertices(glm::mat4 model_matrix)
	{
		int size = vertices_.size();
		assert(size == local_vertices_.size());
		
		for (int i = 0; i < size; i++)
		{
			auto vertex = glm::vec4(local_vertices_[i].x, local_vertices_[i].y, local_vertices_[i].z, 1.0f);
			auto v4 = model_matrix * vertex;
			vertices_[i] = glm::vec3(v4.x, 0, v4.z);

			std::cout << local_vertices_[i].x << " " << local_vertices_[i].z << "\n";
			std::cout << vertices_[i].x << " " <<vertices_[i].z << "\n\n";
		}

	}
	
	int ConvexHull::FindFurthestPoint(glm::vec3 direction)
	{
		int return_value = 0;
		float maxproj = -FLT_MAX;
		
		for (int i = 0; i < local_vertices_.size(); i++)
		{
			float proj = glm::dot(local_vertices_[i], direction);
			if (proj > maxproj)
			{
				maxproj = proj;
				return_value = i;
			}
		}

		return return_value;
	}
}