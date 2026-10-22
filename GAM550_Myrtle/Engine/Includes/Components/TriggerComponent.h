/*!****************************************************************************
 * file  : TriggerComponent.h
 * brief :  This class creates a cause and effect behavior that would
 *          link two gameobjects together.
 * author: Salam Sibai
 * Course: GAM550
 * date  : November 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once
#include "Component.h"
#include "Entities/GameObject.h"
#include "Physics/PhysicalRigidBody.h"


class TriggerComponent : public Component
{
private:
    GameObject* other;
public:
    TriggerComponent(GameObject* _owner, std::string _other);
    void    action();

    static void registerWithFactory(sol::state& lua);
    static void bindToLua(sol::state& lua);

    void    setOther(const std::string& _other);
    void    setOther(PhysicalRigidBody* other_body);
    void    setOther(GameObject* _other);

};
