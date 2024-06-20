#include "../headers/SkullMinionManager.h"
SkullMinionManager* SkullMinionManager::i_ = nullptr;

SkullMinionManager::SkullMinionManager()
{
}

std::shared_ptr<components::SkullMinion> SkullMinionManager::CreateSkullMinion(std::shared_ptr<GameObject> game_object)
{
    std::shared_ptr<components::SkullMinion> component = std::make_shared<components::SkullMinion>();
    component->generator_ = make_shared<pbd::BasicGenerator>();
    component->partcile_ = game_object->GetComponent<components::PBDParticle>();
    component->transform_ = game_object->transform_;
    component->health_component_ = game_object->GetComponent<components::HealthComponent>();
    pbd::PBDManager::i_->CreateFGRRecord(component->partcile_, component->generator_);

    float min_pos = FLT_MAX;
    std::shared_ptr<components::Transform> closest = nullptr;

    glm::vec3 minion_position = game_object->transform_->get_position();

    for (auto& enemy : room_->enemies->transform_->children_)
    {
        glm::vec3 pos = enemy->get_position();

        float len = glm::length2(minion_position - pos);

        if (min_pos > len && len > 1.5f)
        {
            min_pos = len;
            closest = enemy;
        }
    }

    AddMinion(component);
    return component;
}

void SkullMinionManager::AddMinion(std::shared_ptr<components::SkullMinion> minion)
{
	minions_.push_back(minion);
}

void SkullMinionManager::RemoveMinion(std::shared_ptr<components::SkullMinion> minion)
{
    auto it = std::find(minions_.begin(), minions_.end(), minion);
    if (it != minions_.end())
    {
        minions_.erase(it);
    }
}

void SkullMinionManager::UpdateMinion(std::shared_ptr<components::SkullMinion> minion)
{
    float min_pos = FLT_MAX;
    std::shared_ptr<components::Transform> closest = nullptr;

    glm::vec3 minion_position = minion->transform_->get_position();

    for (auto& enemy : room_->enemies->transform_->children_)
    {
        glm::vec3 pos = enemy->get_position();

        float len = glm::length2(minion_position - pos);

        if (min_pos > len)
        {
            min_pos = len;
            closest = enemy;
        }
    }

    if (closest != nullptr)
    {
        minion->target_ = closest->game_object_;
    }

    if (minion->target_ != nullptr)
    {
        

        if (glm::length2(minion_position - minion->target_->transform_->get_position()) < 1.1f)
        {
            auto hc = minion->target_->GetComponent<components::HealthComponent>();
            if (hc != nullptr)
            {
                cout << "minion attack" << endl;
                cout << minion->target_->GetComponent<components::HealthComponent>()->health_ << endl;
                minion->target_->GetComponent<components::HealthComponent>()->TakeDamage(1.0f);
                minion->health_component_->TakeDamage(1.0f);

                cout << minion->target_->GetComponent<components::HealthComponent>()->health_ << endl;
            }
            else
            {
                minion->target_ = nullptr;
            }
            
        }
        else
        {
            //follow
            cout << "minion follow" << endl;
            glm::vec3 follow_point = minion->target_->transform_->get_position();
            glm::vec3 direction = glm::normalize(follow_point - minion_position);

            minion->generator_->direction_ = direction;
            minion->generator_->magnitude_ = 140.0f;
        }
    }
}

void SkullMinionManager::UpdateMinions()
{
    for (auto& m : minions_)
    {
        if (m->active_)
        {
            UpdateMinion(m);
        }
    }
}
