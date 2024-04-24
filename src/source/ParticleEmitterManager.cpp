#include "../headers/ParticleEmitterManager.h"

ParticleEmitterManager *ParticleEmitterManager::i_ = nullptr;

void ParticleEmitterManager::Update(float delta_time)
{
    for (auto &timer : emitter_timers_)
    {
        Timer::UpdateTimer(*timer, delta_time);
    }
}
