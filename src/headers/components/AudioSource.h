#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "../GameObject.h"
#include "../audio/AudioManager.h"
#include "../audio/Sounds.h"

namespace components
{

class AudioSource : public Component
{
private:
    ALuint source_;

public:
    AudioSource();

    void PlaySound(audio::Sounds sound);

    void Start() override {}
    void Update() override {}
    void Destroy() override {}
};

}; // namespace components

#endif // AUDIOSOURCE_H