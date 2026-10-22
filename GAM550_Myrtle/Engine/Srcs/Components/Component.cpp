/*!****************************************************************************
 * file  : Component.cpp
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include "pch.h"
#include "Components/Component.h"
#include "Entities/GameObject.h"

/**
 * @brief Initializes the component.
 * Called once when the component is first created or added.
 */
void Component::initialize()
{
}

/**
 * @brief Updates the component every game tick.
 * Override to define component-specific update behavior.
 */
void Component::update()
{
}


/**
 * @brief Shuts down the component, releasing resources.
 */
void Component::shutdown()
{
}

/**
 * @brief Sets the owner GameObject of this component.
 * @param _owner Shared pointer to the owning GameObject.
 */
void Component::setOwner(GameObject* _owner)
{
    owner = _owner;
}
