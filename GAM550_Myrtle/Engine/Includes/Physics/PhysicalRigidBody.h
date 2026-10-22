/*!****************************************************************************
 * file  : PhysicalRigidBody.h
 * brief : this class is a warper of body class in the Jolt physics engine
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/EActivation.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PhysicsManager;
class PhysicsBodyBuilder;
class PhysicsComponent;

using namespace JPH;

enum CollsionShape {
    BOX,
    SPHERE,
    CAPSULE,
    CONVEX,
    TRIANGLE_MESH
};

enum class PhysicsLayer : uint16 {
    NON_MOVING = 0,
    MOVING = 1,
    PLAYER = 2,
    TRIGGER = 3,
    BULLET = 4,
    BOX = 5,
    NUM_LAYERS = 6
};

namespace Layers
{
    static constexpr ObjectLayer NON_MOVING = 0;
    static constexpr ObjectLayer MOVING = 1;
    static constexpr ObjectLayer PLAYER = 2;
    static constexpr ObjectLayer TRIGGER = 3;
    static constexpr ObjectLayer BULLET = 4;
    static constexpr ObjectLayer BOX = 5;
    static constexpr uint NUM_LAYERS = 6;
};

enum class ActivationMode {
    Activate = static_cast<int>(JPH::EActivation::Activate),
    DontActivate = static_cast<int>(JPH::EActivation::DontActivate)
};

enum class MotionType {
    Static = static_cast<int>(JPH::EMotionType::Static),
    Kinematic = static_cast<int>(JPH::EMotionType::Kinematic),
    Dynamic = static_cast<int>(JPH::EMotionType::Dynamic)
};

inline glm::vec3 toGLMVec3(const JPH::Vec3& v) {
    return glm::vec3(v.GetX(), v.GetY(), v.GetZ());
}

inline JPH::Vec3 toJPHVec3(const glm::vec3& v) {
    return JPH::Vec3(v.x, v.y, v.z);
}

inline glm::quat toGLMQuat(const JPH::Quat& q) {
    return glm::quat(q.GetW(), q.GetX(), q.GetY(), q.GetZ());
}

inline JPH::Quat toJPHQuat(const glm::quat& q) {
    return JPH::Quat(q.x, q.y, q.z, q.w);
}

inline glm::mat4 toGLMMat4(const JPH::Mat44& m) {
    glm::mat4 result;
    for (int col = 0; col < 4; ++col) {
        JPH::Vec4 column = m.GetColumn4(col);
        result[col] = glm::vec4(column.GetX(), column.GetY(), column.GetZ(), column.GetW());
    }
    return result;
}

inline JPH::Mat44 toJPHMat44(const glm::mat4& m) {
    JPH::Mat44 result;
    for (int col = 0; col < 4; ++col) {
        result.SetColumn4(col, JPH::Vec4(m[col].x, m[col].y, m[col].z, m[col].w));
    }
    return result;
}

inline JPH::EMotionType toJPHMotion(MotionType type) {
    switch (type) {
    case MotionType::Static: return EMotionType::Static;
    case MotionType::Kinematic: return EMotionType::Kinematic;
    case MotionType::Dynamic: return EMotionType::Dynamic;
    default: return EMotionType::Static;
    }
}

inline MotionType toMotionType(JPH::EMotionType type) {
    return static_cast<MotionType>(static_cast<int>(type));
}

inline JPH::EActivation toJPHActivation(ActivationMode mode) {
    return static_cast<JPH::EActivation>(static_cast<int>(mode));
}

inline ActivationMode toActivationMode(JPH::EActivation mode) {
    return static_cast<ActivationMode>(static_cast<int>(mode));
}

struct CollisionInfo {
    JPH::BodyID bodyA;
    JPH::BodyID bodyB;
    JPH::Vec3 contactPoint;
    JPH::Vec3 contactNormal;
    float penetrationDepth;
    void* userDataA = nullptr;
    void* userDataB = nullptr;
};


class PhysicalRigidBody
{
    friend class PhysicsBodyBuilder; // only builder can construct
    friend class PhysicsManager; // use to add body to the world
public:
    // Transform & Motion
    void setPosition(const glm::vec3& pos, ActivationMode act = ActivationMode::Activate);
    void setRotation(const glm::quat& rot, ActivationMode act = ActivationMode::Activate);
    void setTransform(const glm::vec3& pos, const glm::quat& rot, ActivationMode act = ActivationMode::Activate);
    glm::vec3 getPosition() const;
    glm::quat getRotation() const;
    glm::mat4 getWorldTransform() const;

    // Velocity & Forces
    void setLinearVelocity(const glm::vec3& vel);
    void setAngularVelocity(const glm::vec3& vel);
    void addForce(const glm::vec3& force);
    void addForceAtPoint(const glm::vec3& force, const glm::vec3& point);
    void addTorque(const glm::vec3& torque);
    void addImpulse(const glm::vec3& impulse);
    void addImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& point);

    glm::vec3 getLinearVelocity() const;
    glm::vec3 getAngularVelocity() const;

    // Activation & Sleep
    void activate();
    void deactivate();
    bool isActive() const;
    void resetSleepTimer();

    // Properties & Metadata
    MotionType getMotionType() const;
    void setMotionType(MotionType type, ActivationMode act = ActivationMode::Activate);
    uint64_t getUserData() const;
    void setUserData(uint64_t data);

    // Shape & Collision
    // need a wrapper for shape
    // for now, just assume shape is all AABB
    // maybe we should not allow changing shape and mass after creation?
    //const JPH::Shape* GetShape() const;
    

    // forget what you know about this body’s recent contacts—recalculate them fresh next frame
    void invalidateContactCache();

    //JPH::TransformedShape GetTransformedShape() const;

    // Advanced Utilities
    glm::vec3 getPointVelocity(const glm::vec3& point) const;
    glm::mat4 getInverseInertia() const;
    //float GetInverseMass() const;
    //float GetMass() const;
    //JPH::AABox GetWorldSpaceBounds() const;
    void DispatchCollision(const CollisionInfo& info, PhysicalRigidBody* other);

    void setOwner(PhysicsComponent*);

    BodyID getBodyID() const { return mID; };
    PhysicsComponent* getOwnerComponent() { return m_ownerComponent; }
private:
    

    // not allow to change shape after creation
    // void SetShape(const JPH::Shape* shape, bool updateMassProperties = true, ActivationMode act = ActivationMode::Activate);

    PhysicalRigidBody(JPH::BodyID id, BodyInterface& iface)
        : m_ownerComponent(nullptr), mID(id), mBodyInterface(iface) {}
    PhysicsComponent* m_ownerComponent;
    BodyID mID;
    BodyInterface& mBodyInterface;
};
