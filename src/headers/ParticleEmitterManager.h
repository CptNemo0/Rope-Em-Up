#ifndef PARTICLEEMITTERMANAGER_H
#define PARTICLEEMITTERMANAGER_H

#include <vector>

#include "global.h"
#include "components/ParticleEmitter.h"
#include "Timer.h"
#include "Camera.h"

namespace components
{
    class ParticleEmitter;
}

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
    s_ptr<llr::Camera> camera;
    std::vector<s_ptr<Timer::Timer>> emitter_timers_;
    std::vector<s_ptr<components::ParticleEmitter>> emitters_;

    void Update(float delta_time);
    void Draw();
};

#endif // PARTICLEEMITTERMANAGER_H