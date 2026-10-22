/*!****************************************************************************
 * file  : EventManager.cpp
 * brief : Implementation of EventManager class
 *
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#include "pch.h"
#include "EventSystem\EventManager.h"

void EventManager::addListener(IEventListener *listener)
{
    listeners.push_back(listener);
}

void EventManager::removeListener(IEventListener *listener)
{
    listeners.erase(
        std::remove(listeners.begin(), listeners.end(), listener),
        listeners.end());
}
