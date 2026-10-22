/*!****************************************************************************
 * file  : Component.h
 * brief : Component - Components define modular behaviors or data that can be attached
 * to a GameObject. This class provides a standard interface for
 * initialization, updating, rendering, serialization, and shutdown
 * 
 * 
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include <memory>
#include <string>
#include <sol/sol.hpp>

//#include "Entities/GameObject.h"

class GameObject;
/**
 * @class Component
 * @brief Base class for all components that can be attached to a GameObject.
 *
 * The Component class provides a common interface for all game object components,
 * including lifecycle methods, serialization, and optional rendering. Components
 * define behavior, appearance, or data for a GameObject and can be extended
 * to implement specific functionality.
 */
class Component
{
public:
    /**
     * @brief Default constructor.
     */
    Component() = default;

    /**
     * @brief Constructs a Component with an owner GameObject.
     * @param _owner Shared pointer to the owning GameObject.
     */
    Component(GameObject* _owner) : owner(_owner) {};
    virtual ~Component() = default;

    void initialize();
    virtual void update();
    void shutdown();
    void setOwner(GameObject* _owner);
    GameObject* getOwner()
    {
        if (owner)
            return owner;
        return nullptr;
    }
protected:

    /** @brief Shared pointer to the GameObject that owns this component. */
    GameObject* owner;
};
