/*!****************************************************************************
 * file  : ParticleComponent.cpp
 * brief : Implementation of ParticleComponent
 *
 * author: Salam Sibai
 * Course: GAM550
 * date  : November 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once
#include "pch.h"
#include "Components/ParticleComponent.h"
#include "Components/ComponentFactory.h"
#include "Entities/GameObject.h"
#include "Memory/AssetLoadManager.h"
#include "FRController/FramerateController.h"

ParticleComponent::ParticleComponent(GameObject* _owner,
    const std::string& texturePath,
    int maxParticles,
    bool autoEmit)
    : autoEmit(autoEmit)
    , emissionRate(10.0f)
    , emissionTimer(0.0f)
    , followOwner(true)
{
    setOwner(_owner);

    // Create emitter
    emitter = std::make_shared<ParticleEmitter>();

    // Set initial position from owner
    if (owner)
    {
        emitter->setPosition(owner->getLocalPosition());
    }

    // Load texture if provided
    if (!texturePath.empty())
    {
        setTexture(texturePath);
    }

    // Register with particle system manager
    ParticleSystemManager::instance().addEmitter(emitter);

   // std::cout << "ParticleComponent created for " << _owner->getName() << std::endl;
}

ParticleComponent::~ParticleComponent()
{
    // Note: ParticleSystemManager holds shared_ptr, so emitter will be cleaned up
    // when all references are gone
   // std::cout << "ParticleComponent destroyed" << std::endl;
}

void ParticleComponent::update()
{
    // Update emitter position to follow GameObject
    if (followOwner && owner)
    {
        emitter->setPosition(owner->getLocalPosition());
    }

    // Auto emission
    if (autoEmit)
    {
        float dt = static_cast<float>(FramerateController::instance().GetFrameTime());
        emissionTimer += dt;

        float emissionInterval = 1.0f / emissionRate;
        while (emissionTimer >= emissionInterval)
        {
            emit();
            emissionTimer -= emissionInterval;
        }
    }
}

void ParticleComponent::emit()
{
    if (emitter)
    {
        emitter->emit();
    }
}

void ParticleComponent::setEmitterPosition(const glm::vec3& pos)
{
    if (emitter)
    {
        emitter->setPosition(pos);
    }
}

void ParticleComponent::setTexture(const std::string& texturePath)
{
    auto texture = AssetLoadManager::instance().loadTextureImage(texturePath);
    if (texture && emitter)
    {
        emitter->setTexture(texture);
    }
}

void ParticleComponent::setTextureAtlas(int totalFrames)
{
    if (emitter)
    {
        emitter->setAtlas(1);
    }
}

// ==================== Lua Binding ====================

void ParticleComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent(
        "ParticleComponent",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {
            // Args: texturePath (optional), maxParticles (optional), autoEmit (optional)

            std::string texturePath = 
               args[0].as<std::string>();

            int maxParticles = 100;
            if (args.size() > 1 && args[1].is<int>())
            {
                maxParticles = args[1].as<int>();
            }

            bool autoEmit = true;
            if (args.size() > 2 && args[2].is<bool>())
            {
                autoEmit = args[2].as<bool>();
            }

            return std::make_shared<ParticleComponent>(owner, texturePath, maxParticles, autoEmit);
        },
        1, // 0 required args
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<ParticleComponent>(comp);
            return sol::make_object(L, derived);
        }
    );

    bindToLua(lua);
}

void ParticleComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<ParticleComponent>("ParticleComponent",
        sol::base_classes, sol::bases<Component>(),

        // Emission control
        "emit", &ParticleComponent::emit,
        "setAutoEmit", &ParticleComponent::setAutoEmit,
        "setEmissionRate", &ParticleComponent::setEmissionRate,
        "setFollowOwner", &ParticleComponent::setFollowOwner,

        // Position
        "setEmitterPosition", sol::overload(
            [](ParticleComponent& self, const glm::vec3& pos) {
                self.setEmitterPosition(pos);
            },
            [](ParticleComponent& self, float x, float y, float z) {
                self.setEmitterPosition(glm::vec3(x, y, z));
            }
        ),

        // Appearance
        "setTexture", &ParticleComponent::setTexture,
        "setTextureAtlas", &ParticleComponent::setTextureAtlas
    );
}
