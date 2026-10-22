/*!****************************************************************************
 * file  : AudioComponent.h
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : November 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once
#include "Components/Component.h"
#include "Audio/AudioManager.h"


class AudioComponent : public Component
{
private:
    std::string path;
    std::string name;
    bool        isSFX;

public:
    AudioComponent(const std::string& _path, const std::string& _name, bool sfx);

    void playSound()
    {
        if (isSFX)
            AudioManager::instance().playSFX(name);
    }
    void playBGMusic()
    {
        if (!isSFX)
            AudioManager::instance().playBGM(name);
    }
    void loadSFX(const std::string& filePath, const std::string& soundName)
    {
        //std::cout << "loaded sfx!\n";
        AudioManager::instance().loadSoundSFX(filePath, soundName);
    }
    void loadMusic(const std::string& filePath, const std::string& soundName)
    {
        AudioManager::instance().loadSound(filePath, soundName);
    }

    void bgmVolumeUp()
    {
        AudioManager::instance().bgmVolumeUp();
    }

    void bmgVolumeDown()
    {
        AudioManager::instance().bgmVolumeDown();
    }

    void sfxVolumeDown()
    {
        AudioManager::instance().sfxVolumeDown();
    }

    void sfxVolumeUp()
    {
        AudioManager::instance().sfxVolumeUp();
    }

    void stopBGM()
    {
        AudioManager::instance().stopBGM();
    }

    void stopSFX()
    {
        AudioManager::instance().toggleSFXPause();
    }

    static void registerWithFactory(sol::state& lua);

    static void bindToLua(sol::state& lua);
};

