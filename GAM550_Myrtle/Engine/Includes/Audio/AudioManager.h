
#pragma once
#include "pch.h"
#include "FMODSystem.h"
#include "AudioLoader.h"
#include "AudioPlayer.h"
#include "VolumeController.h"
#include <sol/sol.hpp>

class AudioManager
{
public:
    static AudioManager& instance()
    {
        static AudioManager instance;
        return instance;
    }

   /* static void bindToLua(sol::state& lua)
    {
        lua.new_usertype<AudioManager>("AudioManager",
            "bgmVolumeUp", &AudioManager::bgmVolumeUp,
            "bgmVolumeDown", &AudioManager::bgmVolumeDown,
            "sfxVolumeDown", &AudioManager::sfxVolumeDown,
            "sfxVolumeUp", &AudioManager::sfxVolumeUp,
        lua["AudioManager"] = std::ref(AudioManager::instance());
    }*/

    bool init();
    void update();
    void shutdown();

    bool loadSound(const std::string& filePath, const std::string& soundName) {
        return m_audioLoader->loadSound(filePath, soundName);
        
    }
    bool loadSoundSFX(const std::string& filePath, const std::string& soundName) {
        return m_audioLoader->loadSoundSFX(filePath, soundName);
    }

    void playBGM(const std::string& soundName) {
        m_audioPlayer->playBGM(soundName);
    }

    void playSFX(const std::string& soundName, float volume = 1.0f) {
        m_audioPlayer->playSFX(soundName, volume);
    }

    void stopBGM() {
        m_audioPlayer->stopBGM();
    }

    void stopAllSounds() {
        m_audioPlayer->stopAllSounds();
    }

    /*

    void turnUpVolume() {
        m_volumeController->volumeUp();
    };
    void TurnDownVolume(){
        m_volumeController->volumeDown();
        };


    void setMasterVolume(float volume) {
        m_volumeController->setMasterVolume(volume);
    }

    void setBGMVolume(float volume) {
        m_volumeController->setBGMVolume(volume);
    }

    void setSFXVolume(float volume) {
        m_volumeController.get()->setSFXVolume(volume);
    }

    void mute() { m_volumeController->mute(); }
    void unmute() { m_volumeController->unmute(); }*/

    void toggleMasterPause() {
        m_volumeController->toggleMasterPause();
    }
    bool isMasterPaused() const {
        return m_volumeController->isMasterPaused();
    }

    void toggleBGMPause() {
        m_volumeController->toggleBGMPause();
    }
    void bgmVolumeUp() {
        m_volumeController->bgmVolumeUp();
    }
    void bgmVolumeDown() {
        m_volumeController->bgmVolumeDown();
    }
    bool isBGMPaused() const {
        return m_volumeController->isBGMPaused();
    }
    int getBGMLevel() const {
        return m_volumeController->getBGMLevel();
    }

    void toggleSFXPause() {
        m_volumeController->toggleSFXPause();
    }
    void sfxVolumeUp() {
        m_volumeController->sfxVolumeUp();
    }
    void sfxVolumeDown() {
        m_volumeController->sfxVolumeDown();
    }
    bool isSFXPaused() const {
        return m_volumeController->isSFXPaused();
    }
    int getSFXLevel() const {
        return m_volumeController->getSFXLevel();
    }

    ~AudioManager() = default;
    

    void loadSoundAsync(const std::string& filePath, const std::string& soundName,
        std::function<void(bool)> callback = nullptr) {
        m_audioLoader->loadSoundAsync(filePath, soundName, callback);
    }

    void bindToLua(sol::state& lua)
    {
        lua.new_usertype<AudioManager>("Scene",
            "muteSFX", &AudioManager::toggleSFXPause,
            "muteBGM", &AudioManager::toggleBGMPause);
    }

private:
    //FMOD::System* m_fmodSystem;
    std::unique_ptr<FMODSystem> m_fmodSystem;
    std::unique_ptr<AudioLoader> m_audioLoader;
    std::unique_ptr<AudioPlayer> m_audioPlayer;
    std::unique_ptr<VolumeController> m_volumeController;

    AudioManager() = default;
};

