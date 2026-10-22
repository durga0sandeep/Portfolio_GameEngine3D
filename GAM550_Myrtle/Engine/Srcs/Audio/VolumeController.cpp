

#include "pch.h"
#include "Audio/VolumeController.h"

//VolumeController::VolumeController()
//    : m_fmodSystem(nullptr)
//    , m_masterGroup(nullptr)
//    , m_bgmGroup(nullptr)
//    , m_sfxGroup(nullptr)
//    , m_masterLevel(10)
//    , m_bgmLevel(10)
//    , m_sfxLevel(10)
//    , m_isMuted(false)
//{
//    Volumes[0] = 1.0f;
//    Volumes[1] = 1.0f;
//    Volumes[2] = 1.0f;
//}
//
//void VolumeController::setSystem(FMOD::System* system) {
//    m_fmodSystem = system;
//    if (m_fmodSystem) {
//        m_fmodSystem->getMasterChannelGroup(&m_masterGroup);
//    }
//}
//
//void VolumeController::setChannelGroups(FMOD::ChannelGroup* master,
//    FMOD::ChannelGroup* bgm,
//    FMOD::ChannelGroup* sfx) {
//    m_masterGroup = master;
//    m_bgmGroup = bgm;
//    m_sfxGroup = sfx;
//    applyVolumes();
//}
//
//void VolumeController::setMasterVolume(float volume) {
//    m_masterVolume = glm::clamp(volume, 0.0f, 1.0f);
//    if (m_masterGroup) {
//        m_masterGroup->setVolume(m_masterVolume);
//    }
//}
//
//void VolumeController::setBGMVolume(float volume) {
//    m_bgmVolume = glm::clamp(volume, 0.0f, 1.0f);
//    
//}
//
//void VolumeController::setSFXVolume(float volume) {
//    m_sfxVolume = glm::clamp(volume, 0.0f, 1.0f);
//}
//
//void VolumeController::volumeUp()
//{
//    volume += 10;
//    setMasterVolume(m_masterLevel + 1);
//}
//
//void VolumeController::volumeDown()
//{
//    //volume -= 10;
//    setMasterVolume(m_masterLevel - 1);
//}
//
//void VolumeController::applyVolumes() {
//    if (!m_isMuted) {
//        if (m_masterGroup) {
//            m_masterGroup->setVolume(levelToVolume(m_masterLevel));
//        }
//        if (m_bgmGroup) {
//            m_bgmGroup->setVolume(levelToVolume(m_bgmLevel));
//        }
//        if (m_sfxGroup) {
//            m_sfxGroup->setVolume(levelToVolume(m_sfxLevel));
//        }
//    }
//}
//
//void VolumeController::mute() {
//    if (m_isMuted) return;
//
//    m_isMuted = true;
//
//    // Store current volumes
//    if (m_masterGroup) m_masterGroup->getVolume(&Volumes[0]);
//    if (m_bgmGroup) m_bgmGroup->getVolume(&Volumes[1]);
//    if (m_sfxGroup) m_sfxGroup->getVolume(&Volumes[2]);
//
//    // Pause all groups to save CPU (idle mode)
//    if (m_masterGroup) m_masterGroup->setPaused(true);
//
//    std::cout << "Audio muted (idle mode)" << std::endl;
//}
//
//void VolumeController::unmute() {
//    if (!m_isMuted) return;
//
//    m_isMuted = false;
//
//    // Resume playback
//    if (m_masterGroup) m_masterGroup->setPaused(false);
//
//    // Restore volumes
//    if (m_masterGroup) m_masterGroup->setVolume(Volumes[0]);
//    if (m_bgmGroup) m_bgmGroup->setVolume(Volumes[1]);
//    if (m_sfxGroup) m_sfxGroup->setVolume(Volumes[2]);
//
//    std::cout << "Audio unmuted" << std::endl;
//}

VolumeController::VolumeController()
    : m_fmodSystem(nullptr)
    , m_masterGroup(nullptr)
    , m_bgmGroup(nullptr)
    , m_sfxGroup(nullptr)
    , m_bgmLevel(10)
    , m_sfxLevel(10)
    , m_isMasterPaused(false)
    , m_isBGMPaused(false)
    , m_isSFXPaused(false)
    , m_savedBGMVolume(1.0f)
    , m_savedSFXVolume(1.0f)
{
}

void VolumeController::setSystem(FMOD::System* system) {
    m_fmodSystem = system;
    if (m_fmodSystem) {
        m_fmodSystem->getMasterChannelGroup(&m_masterGroup);
    }
}

void VolumeController::setChannelGroups(FMOD::ChannelGroup* master,
    FMOD::ChannelGroup* bgm,
    FMOD::ChannelGroup* sfx) {
    m_masterGroup = master;
    m_bgmGroup = bgm;
    m_sfxGroup = sfx;

    // Set initial volumes
    if (m_bgmGroup) m_bgmGroup->setVolume(levelToVolume(m_bgmLevel));
    if (m_sfxGroup) m_sfxGroup->setVolume(levelToVolume(m_sfxLevel));
}

void VolumeController::toggleMasterPause() {
    m_isMasterPaused = !m_isMasterPaused;

    if (m_masterGroup) {
        m_masterGroup->setPaused(m_isMasterPaused);
    }

    std::cout << (m_isMasterPaused ? "Master PAUSED" : "Master RESUMED") << std::endl;
}

void VolumeController::toggleBGMPause() {
    // Master pause overrides BGM pause
    if (m_isMasterPaused) {
        std::cout << "Cannot control BGM - Master is paused" << std::endl;
        return;
    }

    m_isBGMPaused = !m_isBGMPaused;
    applyBGMState();
}

void VolumeController::bgmVolumeUp() {
    if (m_isMasterPaused) {
        std::cout << "Cannot control BGM - Master is paused" << std::endl;
        return;
    }

    //m_bgmLevel = glm::clamp(m_bgmLevel + 1, 0, 10);
    if (m_bgmLevel < 100) {
        m_bgmLevel += 10;
    }

    m_isMasterPaused = false;

    applyBGMState();
    //std::cout << "BGM Level: " << m_bgmLevel << "/10" << std::endl;
}

void VolumeController::bgmVolumeDown() {
    if (m_isMasterPaused) {
        std::cout << "Cannot control BGM - Master is paused" << std::endl;
        return;
    }

    if (m_bgmLevel > 0) {
        m_bgmLevel -= 10;
    }
    m_isMasterPaused = false;


    applyBGMState();
    std::cout << "BGM Level: " << m_bgmLevel << "/10" << std::endl;
}

void VolumeController::applyBGMState() {
    /*if (!m_bgmGroup) return;

    if (m_isBGMPaused) {
        m_bgmGroup->setPaused(true);
        std::cout << "BGM PAUSED" << std::endl;
    }
    else {
        m_bgmGroup->setPaused(false);
        m_bgmGroup->setVolume(levelToVolume(m_bgmLevel));
        std::cout << "BGM PLAYING" << std::endl;
    }*/
    m_bgmGroup->setPaused(false);
    float vol = m_bgmLevel / 100.0f;
    FMOD_RESULT result = m_bgmGroup->setVolume(vol);

    if (result != FMOD_OK) {
        std::cout << " setVolume failed: " << FMOD_ErrorString(result) << std::endl;
    }

    if (!m_bgmGroup) {
        std::cout << "m_bgmGroup is NULL!" << std::endl;
        return;
    }

    if (m_isBGMPaused) {
        m_bgmGroup->setPaused(true);
        std::cout << "BGM PAUSED" << std::endl;
    }
    else {
        m_bgmGroup->setPaused(false);
        float vol = m_bgmLevel / 100.0f; 
        FMOD_RESULT result = m_bgmGroup->setVolume(vol);
        std::cout << "BGM PLAYING at " << vol << " (level: " << m_bgmLevel << "/100)" << std::endl;
        if (result != FMOD_OK) {
            std::cout << " setVolume failed: " << FMOD_ErrorString(result) << std::endl;
        }
    }
}


void VolumeController::toggleSFXPause() {
    // Master pause overrides SFX pause
    if (m_isMasterPaused) {
        std::cout << "Cannot control SFX - Master is paused" << std::endl;
        return;
    }

    m_isSFXPaused = !m_isSFXPaused;
    applySFXState();
}

void VolumeController::sfxVolumeUp() {
    if (m_isMasterPaused) {
        std::cout << "Cannot control SFX - Master is paused" << std::endl;
        return;
    }


    m_sfxLevel = glm::clamp(m_sfxLevel +10, 0, 100);
    /*if (m_sfxLevel > 0) {
        m_sfxLevel += 10;
    }*/
    m_isSFXPaused = false;
    applySFXState();
    std::cout << "SFX Level: " << m_sfxLevel << "/10" << std::endl;
}

void VolumeController::sfxVolumeDown() {
    if (m_isMasterPaused) {
        std::cout << "Cannot control SFX - Master is paused" << std::endl;
        return;
    }


    m_sfxLevel = glm::clamp(m_sfxLevel - 10, 0, 100);


   /* if (m_sfxLevel > 0) {
        m_sfxLevel -= 10;
    }*/
    m_isSFXPaused = false;
    applySFXState();
    std::cout << "SFX Level: " << m_sfxLevel << "/10" << std::endl;
}

void VolumeController::applySFXState() {
    if (!m_sfxGroup) return;

    if (m_isSFXPaused) {
        m_sfxGroup->setPaused(true);
        std::cout << "SFX PAUSED" << std::endl;
    }
    else {
        m_sfxGroup->setPaused(false);
        m_sfxGroup->setVolume(levelToVolume(m_sfxLevel));
        //std::cout << "SFX PLAYING" << std::endl;
    }
}
