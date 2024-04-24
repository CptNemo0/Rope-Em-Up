#ifndef PARTICLEEMITTERMANAGER_H
#define PARTICLEEMITTERMANAGER_H

#include <vector>

#include "components/ParticleEmitter.h"
#include "Timer.h"

class ParticleEmitterManager
{
private:
    ParticleEmitterManager() = default;

public:
    static ParticleEmitterManager *i_;

    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new ParticleEmitterManager();
        }
    }

    static void Destroy()
    {
        if (i_ != nullptr)
        {
            delete i_;
            i_ = nullptr;
        }
    }
public:
    std::vector<std::shared_ptr<Timer::Timer>> emitter_timers_;

    void Update(float delta_time);
};

#endif // PARTICLEEMITTERMANAGER_H