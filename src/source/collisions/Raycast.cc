#include "../../headers/collisions/Raycast.h"

std::shared_ptr<GameObject> collisions::Raycast(glm::vec3 start, glm::vec3 dir, float distance, int layer)
{
    if (glm::length2(dir) != 1.0f)
    {
        dir = glm::normalize(dir);
    }

    glm::vec3 end = start + distance * dir;
    glm::vec3 center = start + distance * 0.5f * dir;

    glm::vec3 normal = glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)));

    float half_lenght = distance * 0.5f;

    std::shared_ptr<AABB> aabb = std::make_shared<AABB>();

    aabb->centre = center;
    aabb->extremes = glm::vec3(half_lenght, 0.0f, half_lenght);

    glm::vec3 a = start + normal * kRaycastDX;
    glm::vec3 b = start - normal * kRaycastDX;
    glm::vec3 c = end + normal * kRaycastDX;
    glm::vec3 d = end - normal * kRaycastDX;

    std::vector<glm::vec3> vertices{a, b, c, d};

    glm::mat4 model_matrix = glm::mat4(1.0f);

    std::shared_ptr<ConvexHull> hull = std::make_shared<ConvexHull>();
    hull->local_vertices = vertices;
    hull->vertices = vertices;

    CollisionManager::i_->colliders_;

    std::shared_ptr<GameObject> return_value;
    float rv_distance = INFINITY;

    for (int i = 0; i < CollisionManager::i_->colliders_.size(); i++)
    {
        auto collider = CollisionManager::i_->colliders_[i];

        if (CollisionManager::i_->LayerCheck(collider->layer_, layer))
        {
            if (AABBCollisionCheck(collider->bp_collider_, aabb))
            {
                auto minkowski = std::vector<glm::vec3>();
                auto start_dir_vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                float current_rotation = 0;
                int precision = fmax(collider->np_collider_->vertices.size(), hull->vertices.size());
                int angle = 360 / precision;

                for (int i = 0; i < precision; i++)
                {
                    current_rotation = angle * i;
                    auto rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(current_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
                    auto rotated_vec4 = rotation_matrix * start_dir_vec;
                    auto direction = glm::vec3(rotated_vec4.x, rotated_vec4.y, rotated_vec4.z);

                    minkowski.push_back(SupportRaycast(collider->np_collider_, hull, direction));
                }

                if (InsideDifference(minkowski))
                {
                    auto new_distance = glm::distance(collider->transform_->get_position(), start);
                    if (new_distance < rv_distance)
                    {
                        return_value = collider->gameObject_.lock();
                        rv_distance = new_distance;
                    }
                }
            }
        }
    }
    return return_value;
}
