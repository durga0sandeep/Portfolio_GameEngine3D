#include "pch.h"
#include "Audio/AudioPlayer.h"

AudioPlayer::AudioPlayer() : m_fmodSystem(nullptr), m_audioLoader(nullptr),
m_bgmGroup(nullptr), m_sfxGroup(nullptr) {}

AudioPlayer::~AudioPlayer() {
    stopAllSounds();
}

bool AudioPlayer::initChannelGroups() {
    if (!m_fmodSystem) return false;

    FMOD_RESULT result;

    result = m_fmodSystem->createChannelGroup("BGM", &m_bgmGroup);
    assert(result == FMOD_OK && "Failed to create BGM channel group");

    result = m_fmodSystem->createChannelGroup("SFX", &m_sfxGroup);
    assert(result == FMOD_OK && "Failed to create SFX channel group");

    return true;
}


void AudioPlayer::update() {
    cleanupFinishedChannels();
}

void AudioPlayer::playBGM(const std::string& soundName) {
    if (!m_fmodSystem || !m_audioLoader || !m_bgmGroup) return;


    FMOD::Sound* sound = m_audioLoader->findSound(soundName);
    if (!sound) {
        std::cout << "can't find music: " << soundName << std::endl;
        return;
    }

    FMOD_OPENSTATE state;
    int waitTimeMs = 0;
    const int maxWaitMs = 5000; // 5 seconds timeout

    do {
        sound->getOpenState(&state, nullptr, nullptr, nullptr);
        if (state == FMOD_OPENSTATE_ERROR) {
            std::cout << "Sound failed to load: " << soundName << std::endl;
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        waitTimeMs += 10;
    } while (state != FMOD_OPENSTATE_READY && waitTimeMs < maxWaitMs);

    if (state != FMOD_OPENSTATE_READY) {
        std::cout << "Timeout waiting for sound to load: " << soundName << std::endl;
        return;
    }

    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = m_fmodSystem->playSound(sound, m_bgmGroup, false, &channel);

    if (result == FMOD_OK && channel) {
        channel->setLoopCount(-1);
        //std::cout << "BGM playing: " << soundName << std::endl;
    }
    else {
        std::cout << "Failed to play BGM: " << soundName << " (Error: " << FMOD_ErrorString(result) << ")" << std::endl;
    }
}

void AudioPlayer::playSFX(const std::string& soundName, float volume) {
    std::cout << "playsound" << std::endl;
    if (!m_fmodSystem || !m_audioLoader || !m_sfxGroup) return;

    FMOD::Sound* sound = m_audioLoader->findSound(soundName);
    if (!sound) {
        std::cout << "Can't find SFX: " << soundName << std::endl;
        return;
    }

    FMOD_OPENSTATE state;
    sound->getOpenState(&state, nullptr, nullptr, nullptr);

    if (state == FMOD_OPENSTATE_READY) {
        FMOD::Channel* channel = nullptr;
        FMOD_RESULT result = m_fmodSystem->playSound(sound, m_sfxGroup, false, &channel);
        if (result == FMOD_OK && channel) {
            channel->setVolume(volume);
        }
        else {
            std::cout << "Failed to play SFX: " << soundName << " (Error: " << FMOD_ErrorString(result) << ")" << std::endl;
        }
    }
    else if (state == FMOD_OPENSTATE_LOADING || state == FMOD_OPENSTATE_BUFFERING) {
        std::cout << "SFX not ready yet: " << soundName << std::endl;
    }
    else if (state == FMOD_OPENSTATE_ERROR) {
        std::cout << "SFX failed to load: " << soundName << std::endl;
    }
    std::cout << "playsound" << std::endl;
}



void AudioPlayer::stopSound(const std::string& soundName)
{
    if (m_sfxGroup) m_sfxGroup->stop();
}

void AudioPlayer::stopBGM()
{
    if (m_bgmGroup) m_bgmGroup->stop();
}

void AudioPlayer::stopAllSounds() {
    if (m_bgmGroup) m_bgmGroup->stop();
    if (m_sfxGroup) m_sfxGroup->stop();
}

void AudioPlayer::setSystem(FMOD::System* system) {
    m_fmodSystem = system;
}

void AudioPlayer::setAudioLoader(AudioLoader* loader) {
    m_audioLoader = loader;
}

void AudioPlayer::cleanupFinishedChannels()
{
}
