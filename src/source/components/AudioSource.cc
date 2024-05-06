#include "../../headers/components/AudioSource.h"

components::AudioSource::AudioSource()
{
    alGenSources(1, &source_);
    audio::CheckALError("alGenSources");

    alSourcei(source_, AL_SOURCE_RELATIVE, AL_TRUE);
    alSource3f(source_, AL_POSITION, 0.0f, 0.0f, 0.0f);
}

void components::AudioSource::PlaySound(audio::Sounds sound)
{
    auto buffer = audio::AudioManager::i_->sounds_[sound][0].buffer;
    alSourcei(source_, AL_BUFFER, buffer);
    audio::CheckALError("alSourcei");

    alSourcePlay(source_);
    audio::CheckALError("alSourcePlay");
}
