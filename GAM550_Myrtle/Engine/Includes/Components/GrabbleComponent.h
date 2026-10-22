/*!****************************************************************************
 * file  : @file
 * brief : Grabble hook Component
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once

#include "Physics/PhysicsManager.h"
#include "Components/PhysicsComponent.h"
#include <Jolt/Physics/Constraints/PointConstraint.h>
#include <glm/vec3.hpp>

class GrabbleComponent : public Component{
public:
    GrabbleComponent(GameObject* _owner,PhysicsComponent* ownerPhysics)
        : ownerPhysics(ownerPhysics), staticAnchor(PhysicsBodyBuilder()
            .withMotion(MotionType::Static)   // immovable
            .withLayer(PhysicsLayer::NON_MOVING)          // static collision layer
            .colliderSize(glm::vec3(0))       // or very small, since just need an anchor
            .lockRotation(true)                     // irrelevant for static, but fine
            .build(glm::vec3(0, 0, 0), glm::quat(1.0f, 0.0f, 0.0f, 0.0f))){
        setOwner(_owner);
        PhysicsManager::instance().addStaticBody(staticAnchor);
    }

    // Attach to a swing point (e.g., box in the air)
    void attachToSwingPoint(const glm::vec3& anchorWorld, float distanceReduction);

    // Drag a target body toward the player (player is stationary)
    void DragToPlayer(PhysicsComponent* targetPhysics) {
        //todo
    }

    // Release the constraint
    void detach();

    bool isAttached() const;

    static void registerWithFactory(sol::state& lua);
    static void bindToLua(sol::state& lua);

private:
    PhysicsComponent* ownerPhysics;
    JPH::Constraint* constraint = nullptr;
    JPH::BodyID attachedBody;
    PhysicalRigidBody staticAnchor;
    
};

