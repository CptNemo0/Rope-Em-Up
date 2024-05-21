#include "../headers/ParticleEmitterManager.h"

ParticleEmitterManager *ParticleEmitterManager::i_ = nullptr;

void ParticleEmitterManager::Update(float delta_time)
{
    for (auto &emitter : emitters_)
    {
        if (emitter->active_)
        {
            Timer::UpdateTimer(*(emitter->emitter_timer_), delta_time);
        }
    }
}

void ParticleEmitterManager::Draw()
{
    for (auto &emitter : emitters_)
    {
        if (emitter->active_)
        {
            emitter->DrawParticles();
        }
    }
}
