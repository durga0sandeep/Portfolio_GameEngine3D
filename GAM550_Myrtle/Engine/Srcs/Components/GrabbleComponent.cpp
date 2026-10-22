#include "pch.h"
#include "Components/GrabbleComponent.h"
#include "Physics/PhysicsManager.h"
#include <algorithm>

/**
 * $BRIEF
 * set constrain and move anchor point
 * \param anchorWorld
 * \param distanceReduction
 */
void GrabbleComponent::attachToSwingPoint(const glm::vec3& anchorWorld, float distanceReduction)
{
    detach(); // Remove any existing constraint

    glm::vec3 playerPos = ownerPhysics->getPosition();

    // Calculate the full distance between player and anchor
    float fullDistance = glm::length(anchorWorld - playerPos);

    // Clamp reduction to avoid negative or zero length
    float ropeLength = (((0.1f) > (fullDistance - distanceReduction)) ? (0.1f) : (fullDistance - distanceReduction));

    staticAnchor.setPosition(anchorWorld);

    // Create a fixed-length distance constraint
    constraint = PhysicsManager::instance().createDistanceConstraint(
        ownerPhysics->getBody()->getBodyID(),     // Player body
        staticAnchor.getBodyID(),                // Static anchor (world)
        playerPos,                               // Anchor on player
        anchorWorld,                             // Anchor in world
        ropeLength,                              // Min distance
        ropeLength                               // Max distance
    );

    attachedBody = ownerPhysics->getBody()->getBodyID();
}

/**
 * $BRIEF
 * detach , remove the constrain from system
 */
void GrabbleComponent::detach()
{
    if (constraint) {
        PhysicsManager::instance().removeConstraint(constraint);
        constraint = nullptr;
        attachedBody = JPH::BodyID();
    }
}

/**
 * $BRIEF
 * bool to check if attached(game play logic)
 * \return \b 
 */
bool GrabbleComponent::isAttached() const
{
    return constraint != nullptr;
}

void GrabbleComponent::registerWithFactory(sol::state& lua)
{
}
