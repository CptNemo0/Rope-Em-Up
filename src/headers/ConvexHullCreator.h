#ifndef CONVEX_HULL_CREATOR_H
#define CONVEX_HULL_CREATOR_H

#include "glm/glm.hpp"
#include "Mesh.h"
#include <memory>
#include "ConvexHull.h"
#include "glm/gtc/matrix_transform.hpp"
#include <unordered_set>
namespace collisions
{
    class ConvexHullCreator
    {
    private:
        glm::vec3 GetExtremeAlongDirection(std::shared_ptr<Mesh> mesh, glm::vec3 direction);
        int precision_;
        int angle_;

    public:
        ConvexHullCreator();
        ConvexHullCreator(int precision);
        ~ConvexHullCreator();
        const int get_precision();
        void set_precision(int value);
        std::shared_ptr<ConvexHull> CreateConvexHull(std::shared_ptr<Mesh> mesh);
    };

}



#endif // !CONVEX_HULL_CREATOR_H
