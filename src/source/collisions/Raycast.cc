#include "../../headers/collisions/Raycast.h"

collisions::RaycastHit collisions::Raycast(glm::vec3 start, glm::vec3 dir, float distance, int layer, std::shared_ptr<GameObject> caster)
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

    std::shared_ptr<GameObject> rv_object = nullptr;
    float rv_distance = INFINITY;
    bool imprecise_hit = false;
    for (int i = 0; i < CollisionManager::i_->colliders_.size(); i++)
    {
        auto collider = CollisionManager::i_->colliders_[i];

        if (!(collider->gameObject_.lock() == caster))
        {
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
                            rv_object = collider->gameObject_.lock();
                            rv_distance = new_distance;
                            imprecise_hit = true;
                        }
                    }
                }
            }
        }
    }

    
    bool precise_hit = false;
    //SegmentsIntersectionPoint
    rv_distance = INFINITY;
    glm::vec3 rv_point = glm::vec3(0.0f);
    if (imprecise_hit)
    {
        auto collider = rv_object->GetComponent<components::Collider>();
        
        for (int i = 0; i < gPRECISION; i++)
        {
            glm::vec3 point;
            if (i == gPRECISION - 1)
            {
                point = SegmentsIntersectionPoint(start, end, collider->np_collider_->vertices[i], collider->np_collider_->vertices[0]);
            }
            else
            {
                point = SegmentsIntersectionPoint(start, end, collider->np_collider_->vertices[i], collider->np_collider_->vertices[i + 1]);
            }

            auto d = glm::distance(point, start);
            //d < rv_distance &&
            if (d < distance)
            {
                rv_distance = d;
                rv_point = point;
                precise_hit = true;
            }
        }
    }

    

   

    RaycastHit return_value;
    return_value.object = rv_object;
    return_value.distance = rv_distance;
    return_value.point = rv_point;
    return_value.precise_hit = precise_hit;
    return_value.imprecise_hit = imprecise_hit;

    return return_value;
}

bool collisions::ChokeCheck(std::shared_ptr<GameObject> caster, int precision, int threshold, float distance)
{
    assert((360 % precision) == 0);

    bool return_value = false;

    auto collider = caster->GetComponent<components::Collider>();

    auto forward = caster->transform_->get_forward();
    auto begining = glm::vec3(forward.x, 0.0, forward.z);

    int angle_step = 360 / precision;
    int current_step = angle_step;

    int hits = 0;

    for (int i = 0; i < precision; i++)
    {
        auto direction = glm::rotateY(glm::vec3(forward.x, 0.0f, forward.z), (float)glm::radians((float)(angle_step * i)));
        auto raycast_data = Raycast(caster->transform_->get_position(), direction, distance, collider->layer_, caster);

        if (raycast_data.imprecise_hit)
        {
            hits++;
        }
    }

    if (hits >= threshold)
    {
        return_value = true;
        std::cout << "CHOKED!!!\n";;
    }

    return return_value;
}
