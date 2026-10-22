/*!****************************************************************************
 * file  :  PhysicsComponent.h
 * brief :  Component that adds physics simulation to a GameObject using Jolt 
 *          Physics
 * 
 *          This component wraps a PhysicalRigidBody and synchronizes it with
 *          the GameObject's transform. It provides methods for
 *          applying forces, setting velocity, and handling collisions through 
 *          Lua callbacks.
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once
#include <string>
#include <vector>
#include <functional>
#include "Components/Component.h"
#include "Entities/GameObject.h"
#include <glm/vec3.hpp>
#include "Physics/PhysicsBodyBuilder.h"
#include "Physics/PhysicsManager.h"

class PhysicalRigidBody;
struct CollisionInfo;

class PhysicsComponent : public Component
{
private:
    PhysicsBodyBuilder      builder;
    PhysicalRigidBody       body;
    bool                    has_gravity;
    sol::protected_function luaOnCollision; ///< Lua callback for collision events (protected from GC)
    sol::reference          luaCallbackRef;

public:
    PhysicsComponent(GameObject* _owner, MotionType type, PhysicsLayer layer,
                        glm::vec3 half_extents, glm::vec3 position, glm::quat rotation, bool fix_rotaion = false);
    ~PhysicsComponent()
    {
        //std::cout << "!!! PhysicsComponent DESTRUCTOR called for " << owner->getName()
         //   << " at address: " << this << std::endl;

        // Remove body from physics world BEFORE destroying component
        try {
            PhysicsManager::instance().removeBody(body);
        }
        catch (...) {
            std::cerr << "Error removing physics body in destructor" << std::endl;
        }

        // CRITICAL: Abandon the Lua references to prevent crash during cleanup
        if (luaOnCollision.valid()) {
            luaOnCollision.abandon();
        }
        if (luaCallbackRef.valid()) {
            luaCallbackRef.abandon();
        }
    };
    void update() override;
    void activate();
    void deactivate();

    ///////////////////
    /// Getters:
    glm::vec3 getVelocity() const;
    glm::vec3 getAngularVelocity() const;
    glm::vec3 getPosition() const;
    MotionType getMotionType() const;
    bool isGravityEnabled() const { return has_gravity; }
    bool isActive() const;
    PhysicalRigidBody* getBody();
    GameObject* getOwnerObject();

    ///////////////////
    /// Setters:
    void setVelocity(const glm::vec3& velocity);
    void setAngularVelocity(const glm::vec3& velocity);
    void setPosition(const glm::vec3& position);
    void setGravityEnabled(bool enabled);
    void setMotionType(MotionType type);

    ///////////////////
    /// Physics Methods:
    void addForce(const glm::vec3& force);
    void addTorque(const glm::vec3& torque);
    void addImpulse(const glm::vec3& impulse);
    void setRotation(const glm::quat& rotation);

    ///////////////////
    /// Collision
    void OnCollision(const CollisionInfo& info, PhysicalRigidBody* other);
    void setLuaOnCollision(sol::function func);

    ///////////////////
    /// Lua functions
    static void    registerWithFactory(sol::state& lua);
    static void bindToLua(sol::state& lua);
};

