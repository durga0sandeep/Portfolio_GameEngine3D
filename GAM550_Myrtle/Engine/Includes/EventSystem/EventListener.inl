#include "pch.h"
// EventListener template implementations (after EventManager definition)
#ifndef EVENT_LISTENER_INL
#define EVENT_LISTENER_INL

#include "EventSystem\EventManager.h"

/**
 * $BRIEF
 * Register the listener to the event manager
 *
 * \param event
 *
 */
template <typename T>
EventListener<T>::~EventListener()
{
    EventManager::Instance().RemoveListener(this);
}

/**
 * $BRIEF
 *
 * \param event
 *
 */
/*
template <typename T>
}
*/

/**
 * $BRIEF
 * handle the event, this include check if the event happen and if happen call the listener
 * can be override with special logic
 * \param event
 *
 */
template <typename T>
void EventListener<T>::handleEvent(const Event &event)
{
    if (auto *specificEvent = dynamic_cast<const T *>(&event))
    {
        OnEvent(*specificEvent);
    }
}

#endif
