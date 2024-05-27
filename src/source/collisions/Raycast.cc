#include "../../headers/collisions/Raycast.h"

collisions::RaycastHit collisions::Raycast(glm::vec3 start, glm::vec3 dir, float distance, int layer, s_ptr<GameObject> caster)
{
    RaycastHit return_value;
    return_value.object = nullptr;
    return_value.distance = FLT_MAX;
    return_value.point = glm::vec3(0.0f, 0.0f, 0.0f);
    return_value.hit = false;

    if (glm::length2(dir) != 1.0f)
    {
        dir = glm::normalize(dir);
    }

    glm::vec3 end = start + distance * dir;
    glm::vec3 center = start + distance * 0.5f * dir;

    float half_lenght = distance * 0.5f;

    s_ptr<AABB> aabb = make_shared<AABB>();
    aabb->centre = center;
    aabb->extremes = glm::vec3(half_lenght, 0.0f, half_lenght);

    s_ptr<GameObject> rv_object = nullptr;
    glm::vec3 rv_point = glm::vec3(0.0f);
    float rv_distance = INFINITY;
    bool hit = false;

    for (int i = 0; i < CollisionManager::i_->colliders_.size(); i++)
    {
        auto collider = CollisionManager::i_->colliders_[i];

        if (!collider->active_)
        {
            continue;
        }

        if (!(collider->gameObject_.lock() == caster))
        {
            if (CollisionManager::i_->LayerCheck(collider->layer_, layer))
            {
                if (AABBCollisionCheck(collider->bp_collider_, aabb))
                {
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
                        if (d < distance)
                        {
                            return_value.object = collider->gameObject_.lock();
                            return_value.distance = d;
                            return_value.point = point;
                            return_value.hit = true;
                        }
                    }
                }
            }
        }
    }


    return return_value;
}

bool collisions::ChokeCheck(s_ptr<GameObject> caster, int precision, int threshold, float distance)
{
    assert((360 % precision) == 0);

    bool return_value = false;
    auto collider = caster->GetComponent<components::Collider>();
    auto forward = caster->transform_->get_forward();
    int angle_step = 360 / precision;
    int hits = 0;

    for (int i = 0; i < precision; i++)
    {
        auto direction = glm::rotateY(glm::vec3(forward.x, 0.0f, forward.z), (float)glm::radians((float)(angle_step * i)));
        auto raycast_data = Raycast(caster->transform_->get_position(), direction, distance, collider->layer_, caster);
        hits = hits + (int)raycast_data.hit;
    }

    return_value = (hits >= threshold);

    return return_value;
}
