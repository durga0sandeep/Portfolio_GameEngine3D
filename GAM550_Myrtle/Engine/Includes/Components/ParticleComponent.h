/*!****************************************************************************
 * file  : ParticleComponent.h
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once
#include "Components/Component.h"
#include "Graphics/ParticleSystem/ParticleEmitter.h"
#include "Graphics/ParticleSystem/ParticleSystemManager.h"
#include <memory>
#include <string>

class ParticleComponent : public Component
{
private:
    std::shared_ptr<ParticleEmitter> emitter;
    bool autoEmit;              // Automatically emit particles every frame
    float emissionRate = 100;         // Particles per second
    float emissionTimer;        // Internal timer for emission
    bool followOwner;           // Update emitter position to match GameObject

public:
    /**
     * @brief Constructor
     * @param _owner The GameObject this component is attached to
     * @param texturePath Path to particle texture
     * @param maxParticles Maximum number of particles
     * @param autoEmit Whether to automatically emit particles
     */
    ParticleComponent(GameObject* _owner,
        const std::string& texturePath = "",
        int maxParticles = 100,
        bool autoEmit = false);

    ~ParticleComponent();

    /**
     * @brief Update function called every frame
     */
    void update() override;

    // ==================== Emitter Control ====================

    /**
     * @brief Manually emit particles
     */
    void emit();

    /**
     * @brief Set whether to automatically emit particles
     */
    void setAutoEmit(bool enable) { autoEmit = enable; }

    /**
     * @brief Set emission rate (particles per second)
     */
    void setEmissionRate(float rate) { emissionRate = rate; }

    /**
     * @brief Set whether emitter follows GameObject position
     */
    void setFollowOwner(bool follow) { followOwner = follow; }

    /**
     * @brief Set emitter position manually
     */
    void setEmitterPosition(const glm::vec3& pos);

    // ==================== Texture & Appearance ====================

    /**
     * @brief Set particle texture
     */
    void setTexture(const std::string& texturePath);

    /**
     * @brief Set texture atlas for animated particles
     * @param totalFrames Number of frames in the atlas
     */
    void setTextureAtlas(int totalFrames);

    // ==================== Access ====================

    /**
     * @brief Get the underlying emitter
     */
    std::shared_ptr<ParticleEmitter> getEmitter() { return emitter; }

    // ==================== Lua Binding ====================

    static void registerWithFactory(sol::state& lua);
    static void bindToLua(sol::state& lua);
};
