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
    std::sort(emitters_.begin(), emitters_.end(), [](const s_ptr<components::ParticleEmitter> &a, const s_ptr<components::ParticleEmitter> &b)
    {
        return a->camera_distance_ < b->camera_distance_;
    });
}

void ParticleEmitterManager::RemoveEmitter(s_ptr<components::ParticleEmitter> emitter)
{
    if (emitters_.size() > 0)
    {
        emitters_.erase(std::find(emitters_.begin(), emitters_.end(), emitter));
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
