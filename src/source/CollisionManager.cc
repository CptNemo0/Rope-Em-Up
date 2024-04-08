#include "../headers/CollisionManager.h"

collisions::CollisionManager* collisions::CollisionManager::i_ = nullptr;

collisions::CollisionManager::CollisionManager()
{
    colliders_ = std::vector<std::shared_ptr<Components::Collider>>();
    for (int i = 0; i < 32; i++)
    {
        collision_layers[i] = (1 << i);
    }
}

void collisions::CollisionManager::AddCollider(std::shared_ptr<Components::Collider> collider)
{
    colliders_.push_back(collider);
}

std::shared_ptr<Components::Collider> collisions::CollisionManager::CreateCollider(int layer, int precision, std::shared_ptr<Mesh> mesh, std::shared_ptr<Components::Transform> transform)
{
    assert(layer > -1 && layer < 32);
    auto return_value = std::make_shared<Components::Collider>(layer, precision, mesh, transform);
    AddCollider(return_value);
    return return_value;
}

void collisions::CollisionManager::Separation(std::shared_ptr<Components::Collider> a, std::shared_ptr<Components::Collider> b)
{
    auto separation_vector = GetSeparatingVector(a->np_collider_,
        a->transform_->get_position(),
        b->np_collider_,
        b->transform_->get_position());
    a->transform_->set_position(a->transform_->get_position() + separation_vector.sep_a);
    b->transform_->set_position(b->transform_->get_position() + separation_vector.sep_b);
}

void collisions::CollisionManager::AddCollisionBetweenLayers(int layer_1, int layer_2)
{
    collision_layers[layer_1] = collision_layers[layer_1] | (1 << layer_2);
    collision_layers[layer_2] = collision_layers[layer_2] | (1 << layer_1);
}

void collisions::CollisionManager::RemoveCollisionBetweenLayers(int layer_1, int layer_2)
{
    collision_layers[layer_1] = collision_layers[layer_1] & ~(1 << layer_2);
    collision_layers[layer_2] = collision_layers[layer_2] & ~(1 << layer_1);
}

void collisions::CollisionManager::UpdateColliders()
{
    for (auto& c : colliders_)
    {
        c->UpdateColliders();
    }
}

void collisions::CollisionManager::CollisionCheck()
{
    static float time = 0;
    static int idx = 0;


    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int i = 0; i < colliders_.size() - 1; i++)
    {
        for (int j = i + 1; j < colliders_.size(); j++)
        {
            std::shared_ptr<Components::Collider> a = colliders_[i];
            std::shared_ptr<Components::Collider> b = colliders_[j];

            if (LayerCheck(a->layer_, b->layer_))
            {
                bool are_colliding = AABBCollisionCheck(a->bp_collider_, b->bp_collider_);
                if (are_colliding)
                {
                    are_colliding = ConvexHullCheckFaster2(a->np_collider_, b->np_collider_);
                    if (are_colliding)
                    {
                        Separation(a, b);
                        auto particle_a = a->gameObject_.lock()->GetComponent<Components::Particle>();
                        auto particle_b = b->gameObject_.lock()->GetComponent<Components::Particle>();
                        physics::PhysicsManager::i_->ResolveContact(particle_a, particle_b);
                    }
                }
            }
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    
    time += std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count();
    idx++;

    if (idx == 60)
    {
        //std::cout << "CollisionCheck() time = " << time / idx << "[micro s]" << std::endl;
        idx = 0;
        time = 0.0f;
    }
}