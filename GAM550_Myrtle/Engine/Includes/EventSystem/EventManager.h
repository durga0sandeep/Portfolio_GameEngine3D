/*!****************************************************************************
 * file  : EventManager.h
 * brief : Event Manager - Singleton pattern
 * handle all the boardcast of the event
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once

#include "Event.h"
#include "EventListener.h"

#include <vector>

class EventManager {
public:
    static EventManager& instance() {
        static EventManager instance;
        return instance;
    }

    // Simple version - just store all listeners in a vector
    //void AddListener(EventListener<Event>* listener);
    void addListener(IEventListener* listener);

    //void RemoveListener(EventListener<Event>* listener);
    void removeListener(IEventListener* listener);

    // Broadcast event to interested listeners
    template<typename T>
    void broadcastEvent(const T& event) {
        for (auto listener : listeners) {
            listener->HandleEvent(event);
        }
    }

private:
    EventManager() = default;
    std::vector<IEventListener*> listeners;
};
