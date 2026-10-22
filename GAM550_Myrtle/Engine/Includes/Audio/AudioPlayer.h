#pragma once
#include "pch.h"
#include "Audio/AudioLoader.h"


class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();

    bool initChannelGroups();
    void update(); 

    void playBGM(const std::string& soundName);
    void playSFX(const std::string& soundName, float volume = 1.0f);

    void stopSound(const std::string& soundName);
    void stopBGM();
    void stopAllSounds();

    void setSystem(FMOD::System* system);
    void setAudioLoader(AudioLoader* loader);

    FMOD::ChannelGroup* getBGMGroup() { return m_bgmGroup; }
    FMOD::ChannelGroup* getSFXGroup() { return m_sfxGroup; }

private:
    FMOD::System* m_fmodSystem = nullptr;
    AudioLoader* m_audioLoader;

    FMOD::ChannelGroup* m_bgmGroup;
    FMOD::ChannelGroup* m_sfxGroup;

    void cleanupFinishedChannels();

};

