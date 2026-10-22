#pragma once
#include "pch.h"


class FMODSystem
{
public:
    FMODSystem();
    ~FMODSystem();

    bool init(int maxChannels = 32);
    void update();
    void shutdown();

    FMOD::System* getSystem() const { return m_system; }
    FMOD::ChannelGroup* getChannelGroup() const { return m_channelGroup; }
    bool isInitialized() const { return m_initialized; }

private:
    FMOD::System* m_system;
    FMOD::ChannelGroup* m_channelGroup;
    bool m_initialized;

};

