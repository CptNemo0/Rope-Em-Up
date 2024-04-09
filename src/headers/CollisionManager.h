#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <memory>
#include <vector>
#include <chrono>
#include <ctime>
#include <format>

#include "Collider.h"
#include "Collisions.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Physics.h"
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

        int collision_layers[32];
        std::vector<std::shared_ptr<Components::Collider>> colliders_;
        

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
       
        void CollisionCheck(std::vector<physics::Contact>& contacts);
        void Separation(std::shared_ptr<Components::Collider> a, std::shared_ptr<Components::Collider> b);
        void AddCollisionBetweenLayers(int layer_1, int layer_2);
        void RemoveCollisionBetweenLayers(int layer_1, int layer_2);
        inline bool LayerCheck(int layer_1, int layer_2)
        {
            return collision_layers[layer_1] & (1 << layer_2);
        }
	
        void UpdateColliders();
    };



}

#endif // !COLLISION_MANAGER_H
