#include "../../headers/components/ParticleEmitter.h"

components::ParticleEmitter::ParticleEmitter(std::shared_ptr<tmp::Texture> texture, std::shared_ptr<Shader> shader)
{
    this->texture = texture;
    this->shader = shader;
}

void components::ParticleEmitter::Start()
{
    transform = gameObject_.lock()->transform_;
}

void components::ParticleEmitter::Update()
{
}

void components::ParticleEmitter::Destroy()
{
}
