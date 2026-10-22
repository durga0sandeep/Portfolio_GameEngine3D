/*!****************************************************************************
 * file  : PhysicsComponent.cpp
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : November 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include "pch.h"
#include "Components/PhysicsComponent.h"
#include "Entities/GameObject.h"
#include "Components/ComponentFactory.h"

/**
 * $BRIEF
 * 
 * \param _owner        The GameObject this component is attached to
 * \param type          Motion type (Static, Kinematic, or Dynamic) 
 * \param layer         Physics layer (NON_MOVING or MOVING)
 * \param half_extents  Half-extents of the box collider (full size / 2)
 * \param position      Initial world position
 * \param rotation      Initial world rotation
 */
PhysicsComponent::PhysicsComponent(GameObject* _owner, MotionType type, PhysicsLayer layer,
    glm::vec3 half_extents, glm::vec3 position, glm::quat rotation, bool fix_rotaion)
    : body(PhysicsBodyBuilder()
        .withMotion(type)
        .withLayer(layer)
        .colliderSize(half_extents)
        .lockRotation(fix_rotaion)
        .build(position, rotation))
    , has_gravity(true)
{
    setOwner(_owner);
    body.setOwner(this);
    if (layer == PhysicsLayer::NON_MOVING)
    {
        PhysicsManager::instance().addStaticBody(body);
        has_gravity = false;
    }
    else
        PhysicsManager::instance().addBody(body);

    std::cout << "PhysicsComponent created for " << _owner->getName()
        << " | Type: " << (type == MotionType::Dynamic ? "Dynamic" : "Static") << std::endl;
}

void PhysicsComponent::update()
{
    owner->setLocalPosition(body.getPosition());
    owner->setLocalRotation(body.getRotation());
}

/* ************************  Setters  ************************ */


glm::vec3 PhysicsComponent::getPosition() const
{
    return body.getPosition();
}

glm::vec3 PhysicsComponent::getAngularVelocity() const
{
    return body.getAngularVelocity();
}

glm::vec3 PhysicsComponent::getVelocity() const
{
    return body.getLinearVelocity();
}

/* ************************  Getters  ************************ */

void PhysicsComponent::setPosition(const glm::vec3& position)
{
    body.setPosition(position, ActivationMode::Activate);
}

void PhysicsComponent::setAngularVelocity(const glm::vec3& velocity)
{
    body.setAngularVelocity(velocity);
}

void PhysicsComponent::setVelocity(const glm::vec3& velocity)
{
    body.setLinearVelocity(velocity);
}

MotionType PhysicsComponent::getMotionType() const
{
    return body.getMotionType();
}

bool PhysicsComponent::isActive() const
{
    return body.isActive();
}

PhysicalRigidBody* PhysicsComponent::getBody()
{
    return &body;
}

GameObject* PhysicsComponent::getOwnerObject()
{
    return getBody()->getOwnerComponent()->getOwner();
}

/* ********************  Physics Methods  ******************** */

void PhysicsComponent::addForce(const glm::vec3& force)
{
    body.addForce(force);
}

void PhysicsComponent::addImpulse(const glm::vec3& impulse)
{
    body.addImpulse(impulse);
}

void PhysicsComponent::addTorque(const glm::vec3& torque)
{
    body.addTorque(torque);
}

void PhysicsComponent::setRotation(const glm::quat& rotation)
{
    body.setRotation(rotation, ActivationMode::DontActivate);
    owner->setLocalRotation(glm::eulerAngles(rotation));
}

void PhysicsComponent::setGravityEnabled(bool enabled)
{
    has_gravity = enabled;
    // TODO: Jolt doesn't have per-body gravity enable/disable directly
    // You might need to implement this through the PhysicsManager
    // For now, just store the flag
}

void PhysicsComponent::setMotionType(MotionType type)
{
    body.setMotionType(type, ActivationMode::Activate);

    // Static bodies shouldn't have gravity
    if (type == MotionType::Static)
        has_gravity = false;
}

void PhysicsComponent::activate()
{
    body.activate();
}

void PhysicsComponent::deactivate()
{
    body.deactivate();
}

/* ***********************  Collision  *********************** */


void PhysicsComponent::OnCollision(const CollisionInfo& info, PhysicalRigidBody* other)
{
  //  std::cout << "OnCollision called on PhysicsComponent at address: " << this << std::endl;

    // Validate 'this' pointer is not corrupted
    if (this == nullptr || (uintptr_t)this == 0xFFFFFFFFFFFFFFF7)
    {
        std::cerr << "ERROR: Invalid PhysicsComponent pointer in collision!" << std::endl;
        return;
    }

    // Check if owner GameObject still exists
    if (!owner)
    {
        std::cerr << "ERROR: PhysicsComponent has no owner GameObject!" << std::endl;
        return;
    }

  //  std::cout << "Collision on " << owner->getName() << std::endl;

    // Check if callback is valid and safe to call
    if (luaOnCollision.valid())
    {
        try
        {
          //  std::cout << "Calling Lua collision callback..." << std::endl;

            // Convert collision info to Lua-friendly format
            glm::vec3 contactPoint = toGLMVec3(info.contactPoint);
            glm::vec3 contactNormal = toGLMVec3(info.contactNormal);

            PhysicsComponent* otherComponent = (other != nullptr) ? other->getOwnerComponent() : nullptr;

            // Call Lua callback with collision data
            sol::protected_function_result result = luaOnCollision(
                std::ref(*this),        // This component
                contactPoint,           // Contact point as vec3
                contactNormal,          // Contact normal as vec3
                info.penetrationDepth,  // Penetration depth
                otherComponent          // Other component (can be null)
            );

            // Check if the call succeeded
            if (!result.valid())
            {
                sol::error err = result;
                std::cerr << "Lua collision callback error: " << err.what() << std::endl;
            }
            else
            {
                std::cout << "Lua callback executed successfully" << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception in collision callback: " << e.what() << std::endl;
        }
    }
    else
    {
        std::cout << "No Lua collision callback registered" << std::endl;
    }
}

void PhysicsComponent::setLuaOnCollision(sol::function func)
{
    if (func.valid())
    {
        luaOnCollision = func;
        // Keep a strong reference to prevent garbage collection
        luaCallbackRef = sol::reference(func.lua_state(), func);
    }
    else
    {
        luaOnCollision = sol::nil;
        luaCallbackRef = sol::nil;
    }
}

/* **********************  Lua Binding  ********************** */

void PhysicsComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent(
        "PhysicsComponent",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<PhysicsComponent>
        {
            MotionType motionType = MotionType::Dynamic;
            if (args.size() > 0)
            {
                std::string typeStr = args[0];
                if (typeStr == "static")
                    motionType = MotionType::Static;
                else if (typeStr == "kinematic")
                    motionType = MotionType::Kinematic;
                else if (typeStr == "dynamic")
                    motionType = MotionType::Dynamic;
            }
            PhysicsLayer layer = PhysicsLayer::MOVING;
            if (args.size() > 1)
            {
                std::string layerStr = args[1];
                if (layerStr == "non_moving" || layerStr == "static")
                    layer = PhysicsLayer::NON_MOVING;
                else if (layerStr == "moving" || layerStr == "dynamic")
                    layer = PhysicsLayer::MOVING;
                else if (layerStr == "trigger")
                    layer = PhysicsLayer::TRIGGER;
                else if (layerStr == "player")
                    layer = PhysicsLayer::PLAYER;
                else if (layerStr == "bullet")
                    layer = PhysicsLayer::BULLET;
                else if (layerStr == "box")
                    layer = PhysicsLayer::BOX;
            }
            glm::vec3 halfExtents(1.5f, 1.5f, 1.5f);
            int currentcount = 0;
            if (args.size() > 2)
            {
                // Check if it's a vec3 object
                if (args[2].is<glm::vec3>())
                {
                    halfExtents = args[2].as<glm::vec3>();
                    currentcount = 3;
                }
                // Check if it's three separate floats
                else if (args.size() >= 5 && args[2].is<float>())
                {
                    halfExtents.x = args[2].as<float>();
                    halfExtents.y = args[3].as<float>();
                    halfExtents.z = args[4].as<float>();
                    currentcount = 5;
                }
            }
            bool lockrotate = false;
            if(args.size() >= currentcount)
            if(args[currentcount].is<bool>())
            {
                lockrotate = args[currentcount].as<bool>();
            }
            
            glm::vec3 position = owner->getLocalPosition();
            glm::vec3 rotationEuler = owner->getLocalRotation();
            glm::quat rotation = glm::quat(glm::radians(rotationEuler));

            std::cout << "Creating PhysicsComponent: type="
                << (motionType == MotionType::Dynamic ? "Dynamic" :
                    motionType == MotionType::Static ? "Static" : "Kinematic")
                << ", layer=" << (layer == PhysicsLayer::MOVING ? "Moving" : "Non-Moving")
                << ", halfExtents=(" << halfExtents.x << ", " << halfExtents.y << ", " << halfExtents.z << ")"
                << std::endl;

            return std::make_shared<PhysicsComponent>(owner, motionType, layer,
                halfExtents, position, rotation, lockrotate);
        },
        0,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<PhysicsComponent>(comp);
            return sol::make_object(L, derived);
        }
    );
    bindToLua(lua);
}

void PhysicsComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<PhysicsComponent>("PhysicsComponent",
        sol::base_classes, sol::bases<Component>(),

        // Velocity - returns vec3, accepts vec3 or 3 floats
        "getVelocity", &PhysicsComponent::getVelocity,
        "setVelocity", sol::overload(
            [](PhysicsComponent& self, const glm::vec3& vel) {
                self.setVelocity(vel);
            },
            [](PhysicsComponent& self, float x, float y, float z) {
                self.setVelocity(glm::vec3(x, y, z));
            }
        ),
        "getAngularVelocity", &PhysicsComponent::getAngularVelocity,
        "setAngularVelocity", sol::overload(
            [](PhysicsComponent& self, const glm::vec3& vel) {
                self.setAngularVelocity(vel);
            },
            [](PhysicsComponent& self, float x, float y, float z) {
                self.setAngularVelocity(glm::vec3(x, y, z));
            }
        ),

        // Position - returns vec3, accepts vec3 or 3 floats
        "getPosition", &PhysicsComponent::getPosition,
        "setPosition", sol::overload(
            [](PhysicsComponent& self, const glm::vec3& pos) {
                self.setPosition(pos);
            },
            [](PhysicsComponent& self, float x, float y, float z) {
                self.setPosition(glm::vec3(x, y, z));
            }
        ),

        // Forces - accepts vec3 or 3 floats
        "addForce", sol::overload(
            [](PhysicsComponent& self, const glm::vec3& force) {
                self.addForce(force);
            },
            [](PhysicsComponent& self, float x, float y, float z) {
                self.addForce(glm::vec3(x, y, z));
            }
        ),
        "addImpulse", sol::overload(
            [](PhysicsComponent& self, const glm::vec3& impulse) {
                self.addImpulse(impulse);
            },
            [](PhysicsComponent& self, float x, float y, float z) {
                self.addImpulse(glm::vec3(x, y, z));
            }
        ),
        "addTorque", sol::overload(
            [](PhysicsComponent& self, const glm::vec3& torque) {
                self.addTorque(torque);
            },
            [](PhysicsComponent& self, float x, float y, float z) {
                self.addTorque(glm::vec3(x, y, z));
            }
        ),
        "setRotation", sol::overload(
            [](PhysicsComponent& self, const glm::quat& rot) {
                self.setRotation(rot);
            },
            [](PhysicsComponent& self, float x, float y, float z) {
                // Euler angles version
                glm::quat rot = glm::quat(glm::vec3(x, y, z));
                self.setRotation(rot);
            }
        ),

        // Properties
        "setGravityEnabled", &PhysicsComponent::setGravityEnabled,
        "isGravityEnabled", &PhysicsComponent::isGravityEnabled,
        "setMotionType", [](PhysicsComponent& self, const std::string& typeStr) {
            MotionType type = MotionType::Dynamic;
            if (typeStr == "static") type = MotionType::Static;
            else if (typeStr == "kinematic") type = MotionType::Kinematic;
            else if (typeStr == "dynamic") type = MotionType::Dynamic;
            self.setMotionType(type);
        },
        "getMotionType", [](PhysicsComponent& self) -> std::string {
            switch (self.getMotionType()) {
            case MotionType::Static: return "static";
            case MotionType::Kinematic: return "kinematic";
            case MotionType::Dynamic: return "dynamic";
            default: return "dynamic";
            }
        },

        // Activation
        "activate", &PhysicsComponent::activate,
        "deactivate", &PhysicsComponent::deactivate,
        "isActive", &PhysicsComponent::isActive,
        "getOwnerObject", &PhysicsComponent::getOwnerObject,

        // Collision callback
        "setOnCollision", &PhysicsComponent::setLuaOnCollision
    );
}
