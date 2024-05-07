#include "../../headers/components/AudioSource.h"

components::AudioSource::AudioSource()
{}

void components::AudioSource::PlaySound(audio::Sounds sound)
{
    auto audio_buffer = audio::AudioManager::i_->GetBuffer(sound);
    auto source = audio::AudioManager::i_->GetFreeSource();
    if (source == -1)
    {
        cerr << "No free sources available!\n";
        return;
    }

    alSourcei(source, AL_BUFFER, audio_buffer.buffer);
    audio::AudioManager::i_->PlaySource(source);
}
