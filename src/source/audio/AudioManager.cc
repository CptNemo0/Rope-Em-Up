#include "../../headers/audio/AudioManager.h"

audio::AudioManager *audio::AudioManager::i_ = nullptr;

char *audio::AudioManager::LoadWAV(const string path, AudioBuffer &audio_buffer)
{
    std::ifstream file;
    file.open(path, std::ios::binary);

    if (!file.is_open())
    {
        cerr << "Could not open file " << path << endl;
        return nullptr;
    }
    
    char buf[4];

    // ChunkID (RIFF)
    file.read(buf, 4);
    if (strncmp(buf, "RIFF", 4) != 0)
    {
        cerr << "Invalid WAV file" << endl;
        return nullptr;
    }

    // ChunkSize
    file.read(buf, 4);

    // Format (WAVE)
    file.read(buf, 4);
    if (strncmp(buf, "WAVE", 4) != 0)
    {
        cerr << "Invalid WAV file" << endl;
        return nullptr;
    }

    // Subchunk1ID
    file.read(buf, 4);
    if (strncmp(buf, "fmt ", 4) != 0)
    {
        cerr << "Invalid WAV file" << endl;
        return nullptr;
    }

    // Subchunk1Size
    file.read(buf, 4);
    if (buf[0] != (char)(16))
    {
        cerr << "Invalid WAV file" << endl;
        return nullptr;
    }

    // AudioFormat
    file.read(buf, 2);
    if (buf[0] != (char)(1))
    {
        cerr << "Invalid WAV file" << endl;
        return nullptr;
    }

    // NumChannels
    file.read(buf, 2);
    int no_channels = (int)((unsigned char)buf[0] | ((unsigned char)buf[1] << 8));

    // SampleRate
    file.read(buf, 4);
    int sample_rate = (int)((unsigned char)buf[0] | ((unsigned char)buf[1] << 8) | ((unsigned char)buf[2] << 16) | ((unsigned char)buf[3] << 24));

    // ByteRate
    file.read(buf, 4);

    // BlockAlign
    file.read(buf, 2);

    // BitsPerSample
    file.read(buf, 2);
    int bits_per_sample = (int)((unsigned char)buf[0] | ((unsigned char)buf[1] << 8));

    // Subchunk2ID
    file.read(buf, 4);
    if (strncmp(buf, "data", 4) != 0)
    {
        cerr << "Invalid WAV file" << endl;
        return nullptr;
    }

    // Subchunk2Size
    file.read(buf, 4);
    int data_bytes = (int)((unsigned char)buf[0] | ((unsigned char)buf[1] << 8) | ((unsigned char)buf[2] << 16) | ((unsigned char)buf[3] << 24));

    audio_buffer.size = data_bytes;

    int format = AL_FORMAT_MONO8 + 2*(no_channels-1) + (bits_per_sample/8)-1;
    audio_buffer.format = format;

    audio_buffer.freq = sample_rate;

    // Data
    char *data = new char[data_bytes];
    file.read(data, data_bytes);

    file.close();

    return data;
}

audio::AudioManager::AudioManager()
{
    device_ = alcOpenDevice(nullptr);

    if (device_)
    {
        context_ = alcCreateContext(device_, nullptr);
        alcMakeContextCurrent(context_);
    }
    else
    {
        cerr << "Couldn't load default audio device" << endl;
    }

    // Clear error
    alGetError();

    ALuint sources[NO_SOURCES];
    alGenSources(NO_SOURCES, sources);
    CheckALError("alGenSources");
    for (int i = 0; i < NO_SOURCES; i++)
    {
        free_sources_.push_back(sources[i]);
    }
}

void audio::AudioManager::LoadSound(Sounds sound, const string path)
{
    AudioBuffer ab;

    alGenBuffers(1, &ab.buffer);
    CheckALError("alGenBuffers");

    char *data = LoadWAV(path, ab);

    if (!data)
    {
        cerr << "Couldn't load sound: " << path << endl;
        return;
    }

    alBufferData(ab.buffer, ab.format, data, ab.size, ab.freq);
    CheckALError("alBufferData");

    delete[] data;

    sounds_[sound].push_back(ab);
}

void audio::AudioManager::Update()
{
    for (auto &source : busy_sources_)
    {
        ALint state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        CheckALError("alGetSourcei");

        if (state != AL_PLAYING)
        {
            sources_to_free_.insert(source);
        }
    }
    for (auto &source : sources_to_free_)
    {
        free_sources_.push_back(source);
    }
    std::erase_if(busy_sources_, [this](const ALuint &source)
    {
        return sources_to_free_.find(source) != sources_to_free_.end();
    });
    sources_to_free_.clear();
}

const ALuint audio::AudioManager::GetFreeSource()
{
    if (!free_sources_.empty())
    {
        auto source = free_sources_.at(0);
        free_sources_.pop_front();
        return source;
    }
    else
    {
        return -1;
    }
}

void audio::AudioManager::PlaySource(const ALuint &source)
{
    alSourcePlay(source);
    busy_sources_.push_back(source);
}

audio::AudioBuffer audio::AudioManager::GetBuffer(Sounds sound)
{
    auto buffers = sounds_[sound];
    auto rand_it = random::RandIntExcl(0, buffers.size());
    return buffers[rand_it];
}

void audio::CheckALError(const string info)
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        cerr << "OpenAL error: " << info << " " << error << endl;
    }
}
