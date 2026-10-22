#include "pch.h"
#include "Audio/FMODSystem.h"

FMODSystem::FMODSystem() : m_system(nullptr), m_channelGroup(nullptr), m_initialized(false) {}

FMODSystem::~FMODSystem() {
    shutdown();
}

bool FMODSystem::init(int maxChannels) {
    FMOD_RESULT result = FMOD::System_Create(&m_system);
    if (result != FMOD_OK) {
        std::cout << "FMOD failed create system" << result << std::endl;
        return false;
    }

    result = m_system->init(maxChannels, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
        std::cout << "FMOD init fail" << std::endl;
        return false;
    }

    result = m_system->createChannelGroup("Master", &m_channelGroup);
    if (result != FMOD_OK) {
        std::cout << "FMOD failed create group" << std::endl;
        return false;
    }

    m_initialized = true;
    return true;
}

void FMODSystem::update() {
    if (m_system) {
        m_system->update();
    }
}

void FMODSystem::shutdown() {
    if (m_system) {
        m_system->close();
        m_system->release();
        m_system = nullptr;
        m_channelGroup = nullptr;
        m_initialized = false;
    }
}
