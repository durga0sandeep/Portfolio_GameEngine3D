#pragma once
#include "pch.h"
#include <future>
#include <mutex>
#include <fstream>

class AudioLoader
{
public:
    AudioLoader();
    ~AudioLoader() = default;

    void setSystem(FMOD::System* system);

    bool loadSound(const std::string& filePath, const std::string& soundName);
    void loadSoundAsync(const std::string& filePath, const std::string& soundName,
        std::function<void(bool)> callback = nullptr);

    bool loadSoundSFX(const std::string& filePath, const std::string& soundName);
    FMOD::Sound* findSound(const std::string& soundName) const;

    bool isSoundLoaded(const std::string& soundName) const;


private:
    FMOD::System* m_fmodSystem;
    //std::vector<std::pair<std::string, FMOD::Sound*>> m_loadedSounds;
    std::unordered_map<std::string, FMOD::Sound*> m_loadedSounds;

    std::unordered_map<std::string, std::future<bool>> m_loadingTasks;
    mutable std::mutex m_loadMutex;

};

