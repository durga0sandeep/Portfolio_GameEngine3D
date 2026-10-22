/*!****************************************************************************
 * file  : SkySphereComponent.cpp
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include "pch.h"
#include "Components/SkySphereComponent.h"
#include "Components/ComponentFactory.h"

void SkySphereComponent::registerWithFactory()
{
        ComponentFactory::instance().registerComponent
        ("SkySphereComponent",
            [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
            {
                std::string mesh = args[0];
                std::string texture = args[1];
                return std::make_shared<SkySphereComponent>(owner, mesh, texture);
            },
            2);
}
