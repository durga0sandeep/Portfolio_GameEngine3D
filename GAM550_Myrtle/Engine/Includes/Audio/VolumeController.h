//#pragma once
//#include "pch.h"
//
//
//
//class VolumeController
//{
//public:
//    VolumeController();
//
//    void setSystem(FMOD::System* system);
//    void setChannelGroups(FMOD::ChannelGroup* master, FMOD::ChannelGroup* bgm, FMOD::ChannelGroup* sfx);
//
//    void setMasterVolume(float volume);
//    void setBGMVolume(float volume);
//    void setSFXVolume(float volume);
//
//    int getMasterVolume() const { return m_masterLevel; }
//    int getBGMVolume() const { return m_bgmLevel; }
//    int getSFXVolume() const { return m_sfxLevel; }
//
//    void volumeUp();
//    void volumeDown();
//
//    void mute();
//    void unmute();
//
//private:
//    FMOD::System* m_fmodSystem;
//    FMOD::ChannelGroup* m_masterGroup; FMOD::ChannelGroup* m_bgmGroup;
//    FMOD::ChannelGroup* m_sfxGroup;
//
//    int m_masterLevel;
//    int m_bgmLevel;
//    int m_sfxLevel;
//    bool m_isMuted;
//
//    float volume;
//    float m_masterVolume;
//    float m_bgmVolume;
//    float m_sfxVolume;
//
//    float Volumes[3]; 
//
//    float levelToVolume(int level) const {
//        return static_cast<float>(level) / static_cast<float>(10);
//    }
//
//    void applyVolumes();
//};
//


#pragma once
//#include <FMOD/fmod.hpp>
#include <glm/glm.hpp>
#include <iostream>

class VolumeController {
public:
    VolumeController();

    void setSystem(FMOD::System* system);
    void setChannelGroups(FMOD::ChannelGroup* master, FMOD::ChannelGroup* bgm, FMOD::ChannelGroup* sfx);

    void toggleMasterPause();
    bool isMasterPaused() const { return m_isMasterPaused; }

    void toggleBGMPause();
    void bgmVolumeUp();
    void bgmVolumeDown();
    bool isBGMPaused() const { return m_isBGMPaused; }
    int getBGMLevel() const { return m_bgmLevel; }

    void toggleSFXPause();
    void sfxVolumeUp();
    void sfxVolumeDown();
    bool isSFXPaused() const { return m_isSFXPaused; }
    int getSFXLevel() const { return m_sfxLevel; }

private:
    float levelToVolume(int level) const {
        return glm::clamp(level / 100.0f, 0.0f, 1.0f);
    }

    void applyBGMState();
    void applySFXState();

    FMOD::System* m_fmodSystem;
    FMOD::ChannelGroup* m_masterGroup;
    FMOD::ChannelGroup* m_bgmGroup;
    FMOD::ChannelGroup* m_sfxGroup;

    int m_bgmLevel;
    int m_sfxLevel;

    bool m_isMasterPaused;
    bool m_isBGMPaused;
    bool m_isSFXPaused;

    float m_savedBGMVolume;
    float m_savedSFXVolume;
};
