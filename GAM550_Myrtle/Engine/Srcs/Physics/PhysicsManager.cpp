/*!****************************************************************************
 * file  : physicsManager.cpp
 * brief : implementation of physicsManager class
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#include "pch.h"
#include "Physics\PhysicsManager.h"
#include "Physics\PhysicsBodyBuilder.h"
#include <Jolt/Physics/Constraints/DistanceConstraint.h>

#include <Jolt/Physics/PhysicsSystem.h>

 // All Jolt symbols are in the JPH namespace
using namespace JPH;

// If you want your code to compile using single or double precision write 0.0_r to get a Real value that compiles to double or float depending if JPH_DOUBLE_PRECISION is set or not.
using namespace JPH::literals;

// We're also using STL classes in this example
using namespace std;


/**
 * $BRIEF
 * Callback for traces, connect this to your own trace handler if you have one
 * \param const char* inFMT
 * \param
 */
static void TraceImpl(const char* inFMT, ...)
{
    // Format the message
    va_list list;
    va_start(list, inFMT);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inFMT, list);
    va_end(list);

    // Print to the TTY
    cout << buffer << endl;
}

/**
 * $BRIEF
 * Callback for asserts, connect this to your own assert handler if you have one
 * \param inExpression
 * \param inMessage
 * \param inFile
 * \param inLine
 * \return \b
 */
static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, uint inLine)
{
    // Print to the TTY
    cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << endl;

    // Breakpoint
    return true;
};

/*!****************************************************************************
 * Class that determines if two object layers can collide
 *****************************************************************************/
class ObjectLayerPairFilterImpl : public ObjectLayerPairFilter
{
public:
    virtual bool ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const override
    {
        switch (inObject1)
        {
        case Layers::NON_MOVING:
            // Non-moving collides with: moving, player, bullet (not triggers)
            return inObject2 == Layers::MOVING ||
                inObject2 == Layers::PLAYER ||
                inObject2 == Layers::BULLET;

        case Layers::MOVING:
            // Moving collides with everything except triggers
            return inObject2 != Layers::TRIGGER;

        case Layers::PLAYER:
            // Player collides with: non-moving, moving, triggers, bullets, boxes
            return inObject2 == Layers::NON_MOVING ||
                inObject2 == Layers::MOVING ||
                inObject2 == Layers::TRIGGER ||
                inObject2 == Layers::BULLET ||
                inObject2 == Layers::BOX;

        case Layers::TRIGGER:
            // Triggers only collide with player and boxes (for detection)
            return inObject2 == Layers::PLAYER ||
                inObject2 == Layers::BOX;

        case Layers::BULLET:
            // Bullets collide with: non-moving, moving, player
            return inObject2 == Layers::NON_MOVING ||
                inObject2 == Layers::MOVING ||
                inObject2 == Layers::PLAYER;

        case Layers::BOX:
            // Boxes collide with: non-moving, moving, triggers, player
            return inObject2 == Layers::NON_MOVING ||
                inObject2 == Layers::MOVING ||
                inObject2 == Layers::TRIGGER ||
                inObject2 == Layers::PLAYER;

        default:
            JPH_ASSERT(false);
            return false;
        }
    }
};


/*!****************************************************************************
 * Broadphase Layers
 *****************************************************************************/
namespace BroadPhaseLayers
{
    static constexpr BroadPhaseLayer NON_MOVING(0);
    static constexpr BroadPhaseLayer MOVING(1);
    static constexpr BroadPhaseLayer PLAYER(2);
    static constexpr BroadPhaseLayer TRIGGER(3);
    static constexpr BroadPhaseLayer BULLET(4);
    static constexpr BroadPhaseLayer BOX(5);
    static constexpr uint NUM_LAYERS(6);
};

/*!****************************************************************************
 * BroadPhaseLayerInterface implementation
 *****************************************************************************/
class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
{
public:
    BPLayerInterfaceImpl()
    {
        // Create a mapping table from object to broad phase layer
        mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
        mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
        mObjectToBroadPhase[Layers::PLAYER] = BroadPhaseLayers::PLAYER;
        mObjectToBroadPhase[Layers::TRIGGER] = BroadPhaseLayers::TRIGGER;
        mObjectToBroadPhase[Layers::BULLET] = BroadPhaseLayers::BULLET;
        mObjectToBroadPhase[Layers::BOX] = BroadPhaseLayers::BOX;
    }

    virtual uint GetNumBroadPhaseLayers() const override
    {
        return BroadPhaseLayers::NUM_LAYERS;
    }

    virtual BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer inLayer) const override
    {
        JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
        return mObjectToBroadPhase[inLayer];
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    virtual const char* GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
    {
        switch ((BroadPhaseLayer::Type)inLayer)
        {
        case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING: return "NON_MOVING";
        case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING: return "MOVING";
        case (BroadPhaseLayer::Type)BroadPhaseLayers::PLAYER: return "PLAYER";
        case (BroadPhaseLayer::Type)BroadPhaseLayers::TRIGGER: return "TRIGGER";
        case (BroadPhaseLayer::Type)BroadPhaseLayers::BULLET: return "BULLET";
        case (BroadPhaseLayer::Type)BroadPhaseLayers::BOX: return "BOX";
        default: JPH_ASSERT(false); return "INVALID";
        }
    }
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
    BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

/*!****************************************************************************
 * Class that determines if an object layer can collide with a broadphase layer
 *****************************************************************************/
class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter
{
public:
    virtual bool ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const override
    {
        switch (inLayer1)
        {
        case Layers::NON_MOVING:
            // Non-moving collides with: moving, player, bullet (not triggers)
            return inLayer2 == BroadPhaseLayers::MOVING ||
                inLayer2 == BroadPhaseLayers::PLAYER ||
                inLayer2 == BroadPhaseLayers::BULLET;

        case Layers::MOVING:
            // Moving collides with everything except triggers
            return inLayer2 != BroadPhaseLayers::TRIGGER;

        case Layers::PLAYER:
            // Player collides with: non-moving, moving, triggers, bullets, boxes
            return inLayer2 == BroadPhaseLayers::NON_MOVING ||
                inLayer2 == BroadPhaseLayers::MOVING ||
                inLayer2 == BroadPhaseLayers::TRIGGER ||
                inLayer2 == BroadPhaseLayers::BULLET ||
                inLayer2 == BroadPhaseLayers::BOX;

        case Layers::TRIGGER:
            // Triggers only collide with player and boxes (for detection)
            return inLayer2 == BroadPhaseLayers::PLAYER ||
                inLayer2 == BroadPhaseLayers::BOX;

        case Layers::BULLET:
            // Bullets collide with: non-moving, moving, player
            return inLayer2 == BroadPhaseLayers::NON_MOVING ||
                inLayer2 == BroadPhaseLayers::MOVING ||
                inLayer2 == BroadPhaseLayers::PLAYER;

        case Layers::BOX:
            // Boxes collide with: non-moving, moving, triggers, player
            return inLayer2 == BroadPhaseLayers::NON_MOVING ||
                inLayer2 == BroadPhaseLayers::MOVING ||
                inLayer2 == BroadPhaseLayers::TRIGGER ||
                inLayer2 == BroadPhaseLayers::PLAYER;

        default:
            JPH_ASSERT(false);
            return false;
        }
    }
};


/**
 * $BRIEF
 * constructor of PhysicsManager class, initializes the physics system
 * will create allocator and job system before creating the physics system
 */
PhysicsManager::PhysicsManager()
    :mPhysicsSystem(std::make_unique<PhysicsSystem>())
    , mBodyInterface(mPhysicsSystem->GetBodyInterface())
    , mBroadPhaseLayerInterface(std::make_unique<BPLayerInterfaceImpl>())
    , mObjectVsBroadPhaseLayerFilter(std::make_unique<ObjectVsBroadPhaseLayerFilterImpl>())
    , mObjectLayerPairFilter(std::make_unique<ObjectLayerPairFilterImpl>())
{
    initialize();
}

PhysicsManager& PhysicsManager::instance() {
    static bool allocatorRegistered = false;
    if (!allocatorRegistered) {
        RegisterDefaultAllocator();
        allocatorRegistered = true;
    }
    static PhysicsManager instance;
    return instance;
}

/**
 * $BRIEF
 * destructor of PhysicsManager class, cleans up the physics system
 */
PhysicsManager::~PhysicsManager()
{
    cleanUp();
}

/**
 * $BRIEF
 * initializer for the physics Engine, set up the physics system and memory allocators
 */
void PhysicsManager::initialize()
{
    // Install trace and assert callbacks
    Trace = TraceImpl;
    JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)

        // Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
        // It is not directly used in this example but still required.
        Factory::sInstance = new JPH::Factory();

    // Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
    // If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
    // If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
    RegisterTypes();

    // We need a temp allocator for temporary allocations during the physics update. We're
    // pre-allocating 10 MB to avoid having to do allocations during the physics update.
    // B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
    // If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
    // malloc / free.
    mTempAllocator = make_unique<TempAllocatorImpl>(10 * 1024 * 1024);

    // We need a job system that will execute physics jobs on multiple threads. Typically
    // you would implement the JobSystem interface yourself and let Jolt Physics run on top
    // of your own job scheduler. JobSystemThreadPool is an example implementation.
    unsigned int concurrency = std::thread::hardware_concurrency();
    unsigned int thread_count = concurrency > 1 ? concurrency - 1 : 1;
    mJobSystem = std::make_unique<JobSystemThreadPool>((uint)cMaxPhysicsJobs, (uint)cMaxPhysicsBarriers, thread_count);
    //mJobSystem = std::make_unique<JPH::JobSystemSingleThreaded>();
    //mJobSystem->Init((uint)cMaxPhysicsJobs, (uint)cMaxPhysicsBarriers, thread_count);


    // This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
    const uint cMaxBodies = 1024;

    // This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
    const uint cNumBodyMutexes = 0;

    // This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
    // body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
    // too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
    const uint cMaxBodyPairs = 1024;

    // This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
    // number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
    const uint cMaxContactConstraints = 1024;

    // Create mapping table from object layer to broadphase layer
    // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
    // Also have a look at BroadPhaseLayerInterfaceTable or BroadPhaseLayerInterfaceMask for a simpler interface.
    // BPLayerInterfaceImpl broad_phase_layer_interface;

    // Create class that filters object vs broadphase layers
    // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
    // Also have a look at ObjectVsBroadPhaseLayerFilterTable or ObjectVsBroadPhaseLayerFilterMask for a simpler interface.
    // ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;

    // Create class that filters object vs object layers
    // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
    // Also have a look at ObjectLayerPairFilterTable or ObjectLayerPairFilterMask for a simpler interface.
    // ObjectLayerPairFilterImpl object_vs_object_layer_filter;

    // Now we can create the actual physics system.
    mPhysicsSystem->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, *mBroadPhaseLayerInterface, *mObjectVsBroadPhaseLayerFilter, *mObjectLayerPairFilter);
    onCollsionListener = std::make_unique<PhysicContactListener>(this);
    mPhysicsSystem->SetContactListener(onCollsionListener.get());
}

/**
 * $BRIEF
 * update function for the physics engine, called every frame
 * \param deltaTime : time elapsed since last frame
 */
void PhysicsManager::update(float deltaTime)
{
    if (deltaTime > 1) return;
    simulate(deltaTime);
    // You can now fetch body transforms from the body interface and update your game objects

    // Update the listers of the bodies that moved
    onCollsionListener->ProcessQueuedContacts();
}

/**
 * $BRIEF
 * Add a rigid body to the physics system
 * \param body : the rigid body to add
 */
void PhysicsManager::addBody(PhysicalRigidBody& body)  // Reference!
{
    JPH_ASSERT(!body.mID.IsInvalid());
    mBodyInterface.AddBody(body.mID, EActivation::Activate);
    mBodies[body.mID] = &body;  // Now stores pointer to actual body
    std::cout << "Added body with ID: " << body.mID.GetIndexAndSequenceNumber()
        << " at address: " << &body << std::endl;
}
//void PhysicsManager::addBody(PhysicalRigidBody body)
//{
//    JPH_ASSERT(!body.mID.IsInvalid());
//    mBodyInterface.AddBody(body.mID, EActivation::Activate);
//    mBodies[body.mID] = &body;
//}


/**
 * $BRIEF
 * Add a static rigid body to the physics system
 * \param body : the static rigid body to add
 */
void PhysicsManager::addStaticBody(PhysicalRigidBody& body)  // Reference!
{
    JPH_ASSERT(!body.mID.IsInvalid());
    mBodyInterface.AddBody(body.mID, EActivation::DontActivate);
    mBodies[body.mID] = &body;  // Now stores pointer to actual body
    std::cout << "Added static body with ID: " << body.mID.GetIndexAndSequenceNumber()
        << " at address: " << &body << std::endl;
}


//void PhysicsManager::addStaticBody(PhysicalRigidBody body)
//{
//    JPH_ASSERT(!body.mID.IsInvalid());
//    mBodyInterface.AddBody(body.mID, EActivation::DontActivate);
//    mBodies[body.mID] = &body;
//}

/**
 * $BRIEF
 * Set the gravity of the physics system
 * \param gravity : the gravity vector to set
 */
void PhysicsManager::setGravity(const glm::vec3& gravity) const
{
    mPhysicsSystem->SetGravity(toJPHVec3(gravity));
}

JPH::Constraint* PhysicsManager::createDistanceConstraint(JPH::BodyID bodyA, JPH::BodyID bodyB, const glm::vec3& anchorA, const glm::vec3& anchorB, float minDistance, float maxDistance)
{
    // Convert glm::vec3 to JPH::Vec3
    JPH::Vec3 jphAnchorA(anchorA.x, anchorA.y, anchorA.z);
    JPH::Vec3 jphAnchorB(anchorB.x, anchorB.y, anchorB.z);

    // Configure the constraint settings
    JPH::DistanceConstraintSettings settings;
    settings.mPoint1 = jphAnchorA;
    settings.mPoint2 = jphAnchorB;
    settings.mMinDistance = minDistance;
    settings.mMaxDistance = maxDistance;
    settings.mSpace = JPH::EConstraintSpace::WorldSpace;

    // Get the non-locking body interface for single-threaded access
    const JPH::BodyLockInterface& lockInterface = mPhysicsSystem->GetBodyLockInterfaceNoLock();

    // Get body pointers directly (no locking overhead)
    JPH::Body* bodyPtrA = lockInterface.TryGetBody(bodyA);
    JPH::Body* bodyPtrB = lockInterface.TryGetBody(bodyB);

    if (!bodyPtrA || !bodyPtrB) {
        // Handle error - one or both bodies don't exist
        return nullptr;
    }

    // Create the constraint
    JPH::Constraint* constraint = settings.Create(*bodyPtrA, *bodyPtrB);

    if (!constraint) {
        return nullptr;
    }

    mPhysicsSystem->AddConstraint(constraint);

    return constraint;
}

/**
 * $BRIEF
 * Remove a rigid body from the physics system
 * \param body : the rigid body to remove
 */
void PhysicsManager::removeBody(PhysicalRigidBody body)
{
    auto it = mBodies.find(body.mID);
    if (it != mBodies.end()) {
        if (mBodyInterface.IsAdded(body.mID)) {
            mBodyInterface.RemoveBody(body.mID);
        }
        mBodies.erase(it);  // Remove from map
        // Don't destroy body yet - PhysicsComponent still owns it
    }
}


void PhysicsManager::reset()
{
    for (auto body : mBodies) {
        if (mBodyInterface.IsAdded(body.first)){
            mBodyInterface.RemoveBody(body.first);
        }
        // Don't destroy body yet - PhysicsComponent still owns it
        mBodyInterface.DestroyBody(body.first);
    }
    mBodies.clear();
}

/**
 * $BRIEF
 * Cleanup function for the physics engine, called on destruction
 */
void PhysicsManager::cleanUp()
{
    //for (const auto& body : mBodies) {
    //    // bodyID is your JPH::BodyID
    //    mBodyInterface.RemoveBody(body.second->mID);
    //    mBodyInterface.DestroyBody(body.second->mID);
    //}
    //UnregisterTypes();

        // Copy the body IDs first (don't use pointers that might be dangling)
    std::vector<JPH::BodyID> bodyIDsToRemove;
    bodyIDsToRemove.reserve(mBodies.size());

    for (const auto& pair : mBodies) {
        bodyIDsToRemove.push_back(pair.first);  // Store BodyID, not pointer
    }

    // Now safely remove and destroy
    for (const JPH::BodyID& bodyID : bodyIDsToRemove) {
        if (mBodyInterface.IsAdded(bodyID)) {  // Check if still in physics world
            mBodyInterface.RemoveBody(bodyID);
            mBodyInterface.DestroyBody(bodyID);
        }
    }

    mBodies.clear();  // Clear the map
    UnregisterTypes();
}

/**
 * $BRIEF
 * Handle Contact
 * \param body1
 * \param body2
 * \param manifold
 */
void PhysicsManager::HandleContact(const JPH::BodyID& body1, const JPH::BodyID& body2, const JPH::ContactManifold& manifold)
{
    auto* rigidA = mBodies.at(body1);
    auto* rigidB = mBodies.at(body2);

    if (!rigidA && !rigidB) return;

    CollisionInfo info;
    info.contactPoint = manifold.mRelativeContactPointsOn1[0]; // Example
    info.contactNormal = manifold.mWorldSpaceNormal;
    info.penetrationDepth = manifold.mPenetrationDepth;

    if (rigidA) rigidA->DispatchCollision(info, rigidB);
    if (rigidB) rigidB->DispatchCollision(info, rigidA);
}

/**
 * $BRIEF
 * Simulate the physics world for a given time step
 * \param deltaTime : time step to simulate
 */
void PhysicsManager::simulate(float deltaTime)
{
    mPhysicsSystem->OptimizeBroadPhase();
    mPhysicsSystem->Update(deltaTime, 1, mTempAllocator.get(), mJobSystem.get());
}

void PhysicsManager::removeConstraint(JPH::Constraint* constraint)
{
    mPhysicsSystem->RemoveConstraint(constraint);
}
