#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <fstream>

#include <AL/al.h>
#include <AL/alc.h>

#include "../global.h"

namespace audio
{

struct AudioBuffer
{
    ALuint buffer;
    ALenum format;
    ALsizei size;
    ALsizei freq;
};

void CheckALError(const string info);

class AudioManager
{
// Singleton stuff
private:
    AudioManager();
    ~AudioManager() = default;

    ALCdevice *device_;
    ALCcontext *context_;

    char *LoadWAV(const string path, AudioBuffer &audio_buffer);

public:
    static AudioManager *i_;

    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new AudioManager();
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

    void LoadSound(const string path);
};

}; // namespace audio

#endif // AUDIOMANAGER_H