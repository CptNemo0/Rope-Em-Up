#include "../../headers/components/Component.h"

#include "../../headers/components/Animator.h"
#include "../../headers/components/AudioSource.h"
#include "../../headers/components/EnemyAIComponent.h"
#include "../../headers/ai/EnemyAIManager.h"
#include "../../headers/components/ExpDropComponent.h"
#include "../../headers/components/GrassRenderer.h"
#include "../../headers/components/HealthComponent.h"
#include "../../headers/components/HpDropComponent.h"
#include "../../headers/components/HUDRenderer.h"
#include "../../headers/components/MeshRenderer.h"
#include "../../headers/components/ParticleEmitter.h"
#include "../../headers/components/PlayerController.h"
#include "../../headers/components/SpellSlotComponent.h"
#include "../../headers/components/TextRenderer.h"
#include "../../headers/collisions/Collider.h"
#include "../../headers/collisions/CollisionManager.h"


std::map<string, std::function<s_ptr<Component>(json&, s_ptr<GameObject>)>> Component::component_factory = 
{
    {   // Animator
        typeid(components::Animator).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::Animator>(j); } 
    },
    {   // AudioSource
        typeid(components::AudioSource).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::AudioSource>(); } 
    },
    {   // EnemyAIComponent
        typeid(components::EnemyAIComponent).name(), 
        [](json &j, s_ptr<GameObject> go) { return ai::EnemyAIManager::i_->CreateEnemyAI(go); } 
    },
    {   // ExpDropComponent
        typeid(components::ExpDropComponent).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::ExpDropComponent>(j); } 
    },
    {   // GrassRenderer
        typeid(components::GrassRenderer).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::GrassRenderer>(j); } 
    },
    {   // HealthComponent
        typeid(components::HealthComponent).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::HealthComponent>(j); } 
    },
    {   // HpDropComponent
        typeid(components::HpDropComponent).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::HpDropComponent>(j); } 
    },
    {   // HUDRenderer
        typeid(components::HUDRenderer).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::HUDRenderer>(j); } 
    },
    {   // MeshRenderer
        typeid(components::MeshRenderer).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::MeshRenderer>(j); } 
    },
    {   // ParticleEmitter
        typeid(components::ParticleEmitter).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::ParticleEmitter>(j); } 
    },
    {   // PlayerController
        typeid(components::PlayerController).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::PlayerController>(j); } 
    },
    {   // SpellSlotComponent
        typeid(components::SpellSlotComponent).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::SpellSlotComponent>(j); } 
    },
    {   // TextRenderer
        typeid(components::TextRenderer).name(), 
        [](json &j, s_ptr<GameObject> go) { return make_shared<components::TextRenderer>(j); } 
    },
    {   // Collider
        typeid(components::Collider).name(), 
        [](json &j, s_ptr<GameObject> go) { return collisions::CollisionManager::i_->CreateCollider(j, go); } 
    }
};

s_ptr<Component> Component::Deserialize(json &j, s_ptr<GameObject> go)
{
    return component_factory[j["type"]](j["data"], go);
}
