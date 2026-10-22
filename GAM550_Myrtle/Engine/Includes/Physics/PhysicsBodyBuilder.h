/*!****************************************************************************
 * file  : PhysicsBodyBuilder.h
 * brief : Builder pattern for creating PhysicalRigidBody instances
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include "PhysicsManager.h"
#include "PhysicalRigidBody.h"

class PhysicsBodyBuilder {
public:
    PhysicsBodyBuilder();

    PhysicsBodyBuilder& withMass(float m);
    PhysicsBodyBuilder& withFriction(float f);
    PhysicsBodyBuilder& asSensor(bool s = true);
    PhysicsBodyBuilder& withMotion(MotionType type);
    PhysicsBodyBuilder& withLayer(PhysicsLayer layer);
    PhysicsBodyBuilder& colliderSize(glm::vec3 halfwidth);
    PhysicsBodyBuilder& lockRotation(bool lock = false);
    PhysicsBodyBuilder& asSphere(float radius);
    PhysicsBodyBuilder& asCapsule(float halfHeight, float radius);
    PhysicsBodyBuilder& asCylinder(float halfHeight, float radius);
    //PhysicsBodyBuilder& asTaperedCapsule(float halfHeight, float topRadius, float bottomRadius);
    //PhysicsBodyBuilder& asTaperedCylinder(float halfHeight, float topRadius, float bottomRadius);
    //PhysicsBodyBuilder& asConvexHull(const std::vector<JPH::Vec3>& points);
    //PhysicsBodyBuilder& asMesh(const std::vector<JPH::Vec3>& vertices, const std::vector<uint32_t>& indices);
    //PhysicsBodyBuilder& asHeightField(const HeightFieldData& data); // Custom struct for terrain
    //PhysicsBodyBuilder& asCompound(const std::vector<JPH::Shape*>& subShapes);
    //PhysicsBodyBuilder& asScaledShape(JPH::Shape* baseShape, glm::vec3 scale);
    //PhysicsBodyBuilder& asRotatedTranslatedShape(JPH::Shape* baseShape, glm::vec3 translation, glm::quat rotation);
    //PhysicsBodyBuilder& asOffsetCenterOfMassShape(JPH::Shape* baseShape, glm::vec3 offset);

    
    PhysicalRigidBody build(
        const glm::vec3& pos,
        const glm::quat& rot);
    //test only, will replace the shape class later
    PhysicalRigidBody buildTemp(const JPH::Shape* shape, const glm::vec3& pos, const glm::quat& rot);

private:
    void reset();
    JPH::ObjectLayer toJoltLayer(PhysicsLayer layer) const;

    // Builder state
    float mass = 1.0f;
    float friction = 0.5f;
    bool isSensor = false;
    JPH::EMotionType motionType = JPH::EMotionType::Dynamic;
    JPH::ObjectLayer layer = 0;
    JPH::BodyInterface& bodyInterface;
    JPH::ShapeRefC shape = nullptr;
    bool lock_rotation = true;
};
