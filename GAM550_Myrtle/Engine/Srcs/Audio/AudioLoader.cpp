
#include "pch.h"
#include "Audio/AudioLoader.h"

AudioLoader::AudioLoader() : m_fmodSystem(nullptr) {}

void AudioLoader::setSystem(FMOD::System* system) {
    m_fmodSystem = system;
}

bool AudioLoader::loadSound(const std::string& filePath, const std::string& soundName) {

    if (!m_fmodSystem) return false;

    std::lock_guard<std::mutex> lock(m_loadMutex);

    if (isSoundLoaded(soundName)) {
        return true;
    }
    
    FMOD::Sound* sound = nullptr;
    FMOD_RESULT result = m_fmodSystem->createSound(
        filePath.c_str(),
        FMOD_CREATESTREAM | FMOD_NONBLOCKING | FMOD_LOOP_NORMAL,
        nullptr,
        &sound
    );

    if (result != FMOD_OK || !sound) {
        std::cout << "Failed to load sound: " << filePath << " (Error: " << FMOD_ErrorString(result) << ")" << std::endl;
        return false;
    }


    //m_loadedSounds.push_back(std::make_pair(soundName, sound));
    m_loadedSounds[soundName] = sound;
    return true;
}

bool AudioLoader::loadSoundSFX(const std::string& filePath, const std::string& soundName) {

    if (!m_fmodSystem) return false;

    if (isSoundLoaded(soundName)) {
        return true;
    }

    FMOD::Sound* sound = nullptr;
    FMOD_RESULT result = m_fmodSystem->createSound(
        filePath.c_str(),
        FMOD_DEFAULT | FMOD_NONBLOCKING,
        nullptr,
        &sound
    );

    if (result != FMOD_OK || !sound) {
        std::cout << "Failed to load sound: " << filePath << " (Error: " << FMOD_ErrorString(result) << ")" << std::endl;
        return false;
    }

    m_loadedSounds[soundName] = sound;
    return true;
}
void AudioLoader::loadSoundAsync(const std::string& filePath,
    const std::string& soundName,
    std::function<void(bool)> callback) {
    auto task = std::async(std::launch::async, [this, filePath, soundName, callback]() {
        bool result = this->loadSound(filePath, soundName);

        if (callback) {
            callback(result);
        }

        return result;
        });

    std::lock_guard<std::mutex> lock(m_loadMutex);
    //m_loadedSounds[soundName] = sound;
    m_loadingTasks[soundName] = std::move(task);
}

//FMOD::Sound* AudioLoader::findSound(const std::string& soundName) const {
//    std::lock_guard<std::mutex> lock(m_loadMutex);
//    auto it = m_loadedSounds.find(soundName);
//    return (it != m_loadedSounds.end()) ? it->second : nullptr;
//}

FMOD::Sound* AudioLoader::findSound(const std::string& soundName) const {
    for (const auto& pair : m_loadedSounds) {
        if (pair.first == soundName) {
            return pair.second;
        }
    }
    return nullptr;
}

bool AudioLoader::isSoundLoaded(const std::string& soundName) const {
    return findSound(soundName) != nullptr;
}
