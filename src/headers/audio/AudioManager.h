#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <fstream>
#include <unordered_map>
#include <vector>

#include <AL/al.h>
#include <AL/alc.h>

#include "../global.h"
#include "Sounds.h"

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

    void LoadSound(Sounds sound, const string path);

    std::unordered_map<Sounds, std::vector<AudioBuffer>> sounds_;
};

}; // namespace audio

#endif // AUDIOMANAGER_H