#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <memory>
#include <vector>

#include "Collider.h"
#include "Collisions.h"
#include "Mesh.h"
#include "Transform.h"


class Components::Collider;

namespace collisions
{
	class CollisionManager
	{
    public:
        static CollisionManager* i_;
    private:
        CollisionManager();
        ~CollisionManager() = default;

        std::vector< std::shared_ptr<Components::Collider> > colliders_;

        void AddCollider(std::shared_ptr<Components::Collider> collider);

    public:
        
        static void Initialize()
        {
            if (i_ == nullptr)
            {
                i_ = new CollisionManager();
            }
        }

        static void Destroy()
        {
            if (i_ != nullptr)
            {
                delete i_;
                i_ = nullptr;
            }
        }

        std::shared_ptr<Components::Collider> CreateCollider(int layer, int precision, std::shared_ptr<Mesh> mesh, std::shared_ptr<Components::Transform> transform);

        void CollisionCheck();
	};



}

#endif // !COLLISION_MANAGER_H
