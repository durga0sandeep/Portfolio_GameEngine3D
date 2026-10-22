/*!****************************************************************************
 * file  : PhysicsManager.h
 * brief : A wrapper around Jolt Physics to manage the physics world and bodies
 * It contains a simple mapping from your external handles to Jolt Body
 * Update the physics world each frame and sync transforms back to your components
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include "Physics/PhysicalRigidBody.h"
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Core/JobSystemSingleThreaded.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include "Graphics/DebugRender/PhysicDebugRenderer.h"
#include "Physics/PhysicConotacListener.h"
#include <vector>

typedef unsigned int uint;

enum RigidBodyMotionType
{
    Static,
    Dynamic,
    Kinematic
};

class PhysicsBodyBuilder; // forward declaration
class BPLayerInterfaceImpl; // forward declaration
class ObjectVsBroadPhaseLayerFilterImpl; // forward declaration
class ObjectLayerPairFilterImpl; // forward declaration

class PhysicsManager
{
    friend class PhysicsBodyBuilder;
    friend class PhysicConotacListener;
public:
    static PhysicsManager& instance();
    using PhysicsHandle = uint32_t; // external handle type
    //PhysicsManager();
    ~PhysicsManager();

    void update(float deltaTime);
    // In PhysicsManager.h
    void addBody(PhysicalRigidBody& body);
    void addStaticBody(PhysicalRigidBody& body);
    void setGravity(const glm::vec3& gravity) const;
    JPH::Constraint* createDistanceConstraint(
        JPH::BodyID bodyA,
        JPH::BodyID bodyB,
        const glm::vec3& anchorA,
        const glm::vec3& anchorB,
        float minDistance,
        float maxDistance);
    void removeConstraint(JPH::Constraint* constraint);

    void removeBody(PhysicalRigidBody handle);
    void reset();
    std::unique_ptr<JPH::PhysicsSystem> mPhysicsSystem;
    JPH::BodyInterface& mBodyInterface;
    //void addListener(JPH::BodyID bodyID, JPH::BodyListener* listener);
    void cleanUp();

    void setupdebugRender() {
        mDebugRenderer = std::make_unique<PhysicDebugRenderer>();
        mDebugRenderer->setupResource();
        JPH::DebugRenderer::sInstance = mDebugRenderer.get();
    }

    void debugRender() {
        if (mPhysicsSystem && mDebugRenderer)
        {
            mPhysicsSystem->DrawBodies(DrawSettings, mDebugRenderer.get());
        }
    }

    void setDebugWire(bool wireframe = true)
    {
        DrawSettings.mDrawShapeWireframe = wireframe;
    }

    void setDebugVelocity(bool flag = true)
    {
        DrawSettings.mDrawVelocity = flag;
    }

    void HandleContact(const JPH::BodyID& body1,
        const JPH::BodyID& body2,
        const JPH::ContactManifold& manifold);

private:
    void simulate(float deltaTime);
    void initialize();
    PhysicsManager();
    PhysicsManager(const PhysicsManager&) = delete;
    PhysicsManager& operator=(const PhysicsManager&) = delete;
    JPH::BodyInterface& getBodyInterface() { return mBodyInterface; }

    //std::unique_ptr<JPH::JobSystemThreadPool> mJobSystem = nullptr;
    std::unique_ptr<PhysicContactListener> onCollsionListener;
    std::unique_ptr<PhysicDebugRenderer> mDebugRenderer;
    JPH::BodyManager::DrawSettings DrawSettings;
    std::unique_ptr<JPH::JobSystemThreadPool> mJobSystem = nullptr;
    //std::unique_ptr<JPH::JobSystemSingleThreaded> mJobSystem = nullptr;
    std::unique_ptr<JPH::TempAllocator> mTempAllocator = nullptr;
    std::unique_ptr<BPLayerInterfaceImpl> mBroadPhaseLayerInterface;
    std::unique_ptr<ObjectVsBroadPhaseLayerFilterImpl> mObjectVsBroadPhaseLayerFilter;
    std::unique_ptr<ObjectLayerPairFilterImpl> mObjectLayerPairFilter;
    // The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
    // variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
    //JPH::BodyInterface& mBodyInterface;
    std::unordered_map<JPH::BodyID, PhysicalRigidBody*> mBodies; // mapping from external handle to Jolt Body
};
