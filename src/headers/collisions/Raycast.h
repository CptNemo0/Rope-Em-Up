#ifndef RAYCASTS_H
#define RAYCASTS_H

#include "../Gameobject.h"
#include "Collisions.h"
#include "CollisionManager.h"
namespace collisions
{
    struct RaycastHit
    {
        std::shared_ptr<GameObject> object;
        glm::vec3 point;
        float distance;
        bool precise_hit;
        bool imprecise_hit;
    };

	const float kRaycastDX = 0.01f;

	/*
	* Zwraca trafiony GameObject albo NULLa jeœli nic nie trafi
	*/
    RaycastHit Raycast(glm::vec3 start, glm::vec3 dir, float distance, int layer);

	inline glm::vec3 SupportRaycast(const std::shared_ptr<ConvexHull> hull_a, const std::shared_ptr <collisions::ConvexHull> hull_b, glm::vec3 direction)
	{
		auto vertex_a = FindFarthestPointConvexHull(hull_a, direction);
		auto vertex_b = FindFarthestPointConvexHull(hull_b, -direction);

		return vertex_a - vertex_b;
	}


    inline glm::vec3 SegmentsIntersectionPoint(glm::vec3 p0, glm::vec3 p1, glm::vec3 q0, glm::vec3 q1) 
    {
        auto x1 = p0.x;
        auto y1 = p0.z;

        auto x2 = p1.x;
        auto y2 = p1.z;

        auto x3 = q0.x;
        auto y3 = q0.z;

        auto x4 = q1.x;
        auto y4 = q1.z;

        auto a = (x4 - x3) * (y3 - y1) - (y4 - y3) * (x3 - x1);
        auto b = (x4 - x3) * (y2 - y1) - (y4 - y3) * (x2 - x1);
        auto c = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);

        
        
        if (fabsf(b) < 1e-10f && fabsf(a) >= 1e-10f)
        {
            return glm::vec3(0xffffffff, 0.0f, 0xffffffff);
        }

        if (fabsf(b) < 1e-10f && fabsf(a) < 1e-10f)
        {
            return glm::vec3(0xffffffff, 0.0f, 0xffffffff);
        }

        auto alpha = a / b;
        auto beta = c / b;

        if (0 < beta && beta < 1.0f && 0 < alpha && alpha < 1.0f)
        {
            
            auto x0 = x1 + alpha * (x2 - x1);
            auto y0 = y1 + alpha * (y2 - y1);

            return glm::vec3(x0, 0.0f, y0);
        }

        else
        {
            return glm::vec3(0xffffffff, 0.0f, 0xffffffff);
        }
        
    }
}




#endif