/*!****************************************************************************
 * file  : AudioComponent.cpp
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : November 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/


#pragma once

#include "pch.h"
#include "Components/AudioComponent.h"
#include "Components/ComponentFactory.h"
#include "Entities/GameObject.h"

AudioComponent::AudioComponent(const std::string& _path, const std::string& _name, bool sfx)
    : path(_path), name(_name), isSFX(sfx)
{
    if (isSFX)
        loadSFX(path, name);
    else
        loadMusic(path, name);
}

void AudioComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent
    ("AudioComponent",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {
            std::string p = args[0];
            std::string n = args[1];
            bool        sfx = args[2];

            return std::make_shared<AudioComponent>(p, n, sfx);
        },
        3,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<AudioComponent>(comp);
            return sol::make_object(L, derived);
        }
    );
    bindToLua(lua);
}

void AudioComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<AudioComponent>("AudioComponent",
        sol::base_classes, sol::bases<Component>(),

        // Emission control
        "playSound", &AudioComponent::playSound,
        "playBGM", &AudioComponent::playBGMusic,
        "bgmVolumeUp", & AudioComponent::bgmVolumeUp,
        "bgmVolumeDown",&AudioComponent::bmgVolumeDown,
        "sfxVolumeUp",&AudioComponent::sfxVolumeUp,
        "sfxVolumeDown", & AudioComponent::sfxVolumeDown,
        "stopBGM",&AudioComponent::stopBGM,
        "stopSFX", & AudioComponent::stopSFX
    );
}
