#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include "glm/glm.hpp"
#include <unordered_map>
#include <memory>
#include "Mesh.h"
#include "glm/glm.hpp"
namespace collisions
{
    class ConvexHull
    {
        public:
            std::vector<glm::vec3> local_vertices_;
            std::vector<glm::vec3> vertices_;
            
            void UpdateVertices(glm::mat4 model_matrix);
    };
}

#endif // !CONVEX_HULL_H