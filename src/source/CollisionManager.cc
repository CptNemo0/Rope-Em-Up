#include "../headers/CollisionManager.h"

collisions::CollisionManager* collisions::CollisionManager::i_ = nullptr;

collisions::CollisionManager::CollisionManager()
{
    colliders_ = std::vector<std::shared_ptr<Components::Collider>>();
}

void collisions::CollisionManager::AddCollider(std::shared_ptr<Components::Collider> collider)
{
    colliders_.push_back(collider);
}

std::shared_ptr<Components::Collider> collisions::CollisionManager::CreateCollider(int layer, int precision, std::shared_ptr<Mesh> mesh, std::shared_ptr<Components::Transform> transform)
{
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

            if (a->layer_ == b->layer_)
            {
                bool are_colliding = AABBCollisionCheck(a->bp_collider_, b->bp_collider_);
                if (are_colliding)
                {
                    are_colliding = ConvexHullCheckFaster2(a->np_collider_, b->np_collider_);
                    if (are_colliding)
                    {
                        Separation(a, b);
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
        std::cout << "CollisionCheck() time = " << time / idx << "[micro s]" << std::endl;
        idx = 0;
        time = 0.0f;
    }
}