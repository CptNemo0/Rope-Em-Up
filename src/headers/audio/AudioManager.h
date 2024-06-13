#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <fstream>
#include <unordered_map>
#include <vector>
#include <list>
#include <deque>
#include <unordered_set>

#include <AL/al.h>
#include <AL/alc.h>

#include "../typedef.h"
#include "../Random.h"

#define NO_SOURCES 128

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
    ~AudioManager();

    ALCdevice *device_;
    ALCcontext *context_;

    std::list<ALuint> busy_sources_;
    std::unordered_set<ALuint> sources_to_free_;
    std::deque<ALuint> free_sources_;

    char *LoadWAV(const string path, s_ptr<AudioBuffer> audio_buffer);

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

    s_ptr<AudioBuffer> LoadSound(const string path);
    void Update();
    // Returns -1 if no free source available
    const ALuint GetFreeSource();
    void PlaySound(const s_ptr<AudioBuffer> source);
};

}; // namespace audio

#endif // AUDIOMANAGER_H