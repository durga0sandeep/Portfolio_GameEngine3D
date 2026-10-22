/*!****************************************************************************
 * file  : BoxSuspender.h
 * brief : A component that act like the game object is hold by a rope
 * can detach and reattach to a point
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

class BoxSuspender : public Component
{
public:
    BoxSuspender(GameObject* _owner, const glm::vec3& anchorWorld, float distance)
        : staticAnchor(PhysicsBodyBuilder()
            .withMotion(MotionType::Static)   // immovable
            .withLayer(PhysicsLayer::NON_MOVING)          // static collision layer
            .colliderSize(glm::vec3(0.1f))       // or very small, since just need an anchor
            .lockRotation(true)                     // irrelevant for static, but fine
            .build(anchorWorld, glm::quat(1.0f, 0.0f, 0.0f, 0.0f))) {
        setOwner(_owner);
        if (auto phys = owner->FindComponent<PhysicsComponent>())
            ownerPhysics = phys.get();
        PhysicsManager::instance().addStaticBody(staticAnchor);
        attachToSwingPoint(anchorWorld, distance);
    }

    // Attach to a swing point (e.g., box in the air)
    void attachToSwingPoint(const glm::vec3& anchorWorld, float distanceReduction);

    // Release the constraint
    void detach();

    void update() override;

    bool isAttached() const;

    static void registerWithFactory(sol::state& lua);
    static void bindToLua(sol::state& lua);

private:
    PhysicsComponent* ownerPhysics;
    JPH::Constraint* constraint = nullptr;
    JPH::BodyID attachedBody;
    PhysicalRigidBody staticAnchor;
};

