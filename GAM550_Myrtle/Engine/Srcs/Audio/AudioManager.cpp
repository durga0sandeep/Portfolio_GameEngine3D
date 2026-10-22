
#include "pch.h"
#include "Audio/AudioManager.h"
bool AudioManager::init() {
    m_fmodSystem = std::make_unique<FMODSystem>();
    m_fmodSystem->init();
    if (!m_fmodSystem->init()) {
        return false;
    }

    m_audioLoader = std::make_unique<AudioLoader>();
    m_audioLoader->setSystem(m_fmodSystem->getSystem());

    m_audioPlayer = std::make_unique<AudioPlayer>();
    m_audioPlayer->setSystem(m_fmodSystem->getSystem());
    m_audioPlayer->setAudioLoader(m_audioLoader.get());

    if (!m_audioPlayer->initChannelGroups()) {
        return false;
    }

    m_volumeController = std::make_unique<VolumeController>();
    m_volumeController->setSystem(m_fmodSystem->getSystem());

    FMOD::ChannelGroup* masterGroup = nullptr;
    m_fmodSystem->getSystem()->getMasterChannelGroup(&masterGroup);

    m_volumeController->setChannelGroups(
        masterGroup,
        m_audioPlayer->getBGMGroup(),
        m_audioPlayer->getSFXGroup()
    );

    std::cout << "init complete" << std::endl;
    return true;
}

void AudioManager::update() {
    if (m_fmodSystem) {
        m_fmodSystem->update();
    }
    if (m_audioPlayer) {
        m_audioPlayer->update();
    }
}

void AudioManager::shutdown() {
    m_audioPlayer.reset();
    m_audioLoader.reset();
    m_volumeController.reset();
    m_fmodSystem.reset();
}
