/*!****************************************************************************
 * file  : EventHandlerComponent.h
 * brief : this class is the base class for EventHandler Component
 * Just a container of listener that add to the object
 * This design is refer to the previous game class idea on the event handler
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
//#include "Component.h"
#include <memory>
#include "EventListener.h"

template <typename T>
class EventHandlerComponent 
//    :public Component
{
public:
	EventHandlerComponent(std::shared_ptr<EventListener<T>> _listener) : listener(_listener) {};
	void initialize() = 0;
	void update(float dt) {};
	void shutdown() = 0;
	void configure() = 0;
	void serialize() = 0;
	void deserialize() = 0;
	virtual void handleEvent(T Event) = 0;
	virtual ~EventHandlerComponent() = default;
protected:
	std::shared_ptr<EventListener<T>> listener;
};

