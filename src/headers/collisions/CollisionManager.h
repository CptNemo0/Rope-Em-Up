#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <memory>
#include <vector>
#include <chrono>
#include <ctime>
#include <format>
#include "nlohmann/json.hpp"

#include "Collider.h"
#include "Collisions.h"
#include "../GameObject.h"
#include "../Model.h"
#include "../physics/PBD.h"
#include "../components/Transform.h"
#include "../res/Resources.h"

#include "../typedef.h"

class components::Collider;

namespace collisions
{
    enum LAYERS
    {
        TENTACLE,
        PLAYER,
        ROPE,
        CLUTTER,
        LAMPS,
        SKULL,
        BARREL,
        HITBOX
    };

	class CollisionManager
	{
    public:
        static CollisionManager* i_;
    private:
        CollisionManager();
        ~CollisionManager() = default;

        int collision_layers[32];

        void AddCollider(s_ptr<components::Collider> collider);

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

        std::vector<s_ptr<components::Collider>> colliders_;

        s_ptr<components::Collider> CreateCollider(int layer, int precision, s_ptr<Model> model, int mesh_index, s_ptr<components::Transform> transform);
        s_ptr<components::Collider> CreateCollider(json &j, s_ptr<GameObject> go);
        void RemoveCollider(s_ptr<components::Collider> collider);
    
        void CollisionCheckPBD(std::vector<pbd::Contact>& contacts);
        void Separation(s_ptr<components::Collider> a, s_ptr<components::Collider> b, float wa, float wb);
        void AddCollisionBetweenLayers(int layer_1, int layer_2);
        void RemoveCollisionBetweenLayers(int layer_1, int layer_2);
        inline bool LayerCheck(int layer_1, int layer_2)
        {
            return collision_layers[layer_1] & (1 << layer_2);
        }
	
        void UpdateColliders();
        void PredictColliders();
    };



}

#endif // !COLLISION_MANAGER_H
