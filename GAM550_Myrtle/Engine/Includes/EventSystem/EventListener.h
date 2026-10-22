/*!****************************************************************************
 * file  : EventListener.h
 * brief : Interface for a explicit EventListener that the compiler can recognize
 * Non-templated base interface for listeners
 * so that the EventManager can store them in a single list
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include "Event.h"

// Interface for EventListener
// Non-templated base class for all event listeners
// so that EventManager can store them in a single list
class IEventListener {
public:
    virtual ~IEventListener() = default;
    virtual void handleEvent(const Event& event) = 0;
};


// Forward declaration - There's a "circular reference with EventManager"
class EventManager;


// Base EventListener class that specific Event
template<typename T>
class EventListener : public IEventListener {
public:
    // Default constructor
    EventListener() = default;

    // Virtual destructor - We know we will extend this class
    virtual ~EventListener();

    // Register this listener with the event manager
    void registerListener();

    // Pure virtual function to be implemented by derived classes
    virtual void onEvent(const T& event) = 0;

    // Implementation of IEventListener interface
    void handleEvent(const Event& event) override;
};

#include "EventListener.inl"
