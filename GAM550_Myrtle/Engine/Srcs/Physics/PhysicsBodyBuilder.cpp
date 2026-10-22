#include "pch.h"
#include "Physics/PhysicsBodyBuilder.h"
#include "Physics/PhysicsManager.h"

PhysicsBodyBuilder::PhysicsBodyBuilder():bodyInterface(PhysicsManager::instance().getBodyInterface())
{
    //reset();
}

/**
 * $BRIEF
 * this function sets the mass for the physics body being built.
 * \param m
 * \return \b 
 */
PhysicsBodyBuilder& PhysicsBodyBuilder::withMass(float m) {
    mass = m;
    return *this;
}

/**
 * $BRIEF
 * this function sets the friction value for the physics body being built.
 * \param f
 * \return \b 
 */
PhysicsBodyBuilder& PhysicsBodyBuilder::withFriction(float f) {
    friction = f;
    return *this;
}

/**
 * $BRIEF
 * this function sets whether the physics body being built is a sensor or not.
 * \param s
 * \return \b 
 */
PhysicsBodyBuilder& PhysicsBodyBuilder::asSensor(bool s) {
    isSensor = s;
    return *this;
}

/**
 * $BRIEF
 * this function sets the motion type for the physics body being built.
 * \param type
 * \return \b 
 */
PhysicsBodyBuilder& PhysicsBodyBuilder::withMotion(MotionType type) {
    
    motionType = toJPHMotion(type);
    return *this;
}

/**
 * $BRIEF
 * this function sets the collision layer for the physics body being built.
 * \param l
 * \return \b 
 */
PhysicsBodyBuilder& PhysicsBodyBuilder::withLayer(PhysicsLayer l) {
    layer = static_cast<uint>(l);
    return *this;
}

/**
 * $BRIEF
 * function that set shape to AABB with given halfwidth
 * \param halfwidth
 * \return \b 
 */
PhysicsBodyBuilder& PhysicsBodyBuilder::colliderSize(glm::vec3 halfwidth)
{
    JPH::BoxShapeSettings settings(JPH::Vec3(halfwidth.x, halfwidth.y, halfwidth.z)); //box size
    settings.SetEmbedded();
    ShapeSettings::ShapeResult box_result = settings.Create();

    shape = box_result.Get();
    return *this;
}

PhysicsBodyBuilder& PhysicsBodyBuilder::lockRotation(bool islock)
{
    lock_rotation = islock;
    return *this;
}

/**
 * $BRIEF
 * funtion that set shape to sphere with given radius
 * \param radius
 * \return \b 
 */
PhysicsBodyBuilder& PhysicsBodyBuilder::asSphere(float radius)
{
    JPH::SphereShapeSettings settings(radius); // default radius
    shape = settings.Create().Get();
    return *this;
}

/**
 * $BRIEF
 * this function sets the shape of the physics body being built to a capsule with the 
 * specified half-height and radius.
 * \param halfHeight
 * \param radius
 * \return \b
 */
PhysicsBodyBuilder& PhysicsBodyBuilder::asCapsule(float halfHeight, float radius)
{
    JPH::CapsuleShapeSettings settings(radius, halfHeight * 2.0f); // radius, height
    //settings.SetEmbedded();
    shape = settings.Create().Get();
    return *this;
}

/**
 * $BRIEF
 * this function sets the shape of the physics body being built to a cylinder with 
 * the specified half-height and radius.
 * \param halfHeight
 * \param radius
 * \return \b
 */
PhysicsBodyBuilder& PhysicsBodyBuilder::asCylinder(float halfHeight, float radius)
{
    //TODO: add this
    //JPH::CylinderShapeSetting
    return *this;
}

/**
 * $BRIEF
 * converts a custom PhysicsLayer enum to a Jolt ObjectLayer.
 * \param layer
 * \return \b 
 */
JPH::ObjectLayer PhysicsBodyBuilder::toJoltLayer(PhysicsLayer layer) const {
    switch (layer) {
    case PhysicsLayer::MOVING: return Layers::MOVING;
    case PhysicsLayer::NON_MOVING: return Layers::NON_MOVING;
    case PhysicsLayer::PLAYER: return Layers::PLAYER;
    case PhysicsLayer::TRIGGER: return Layers::TRIGGER;
    case PhysicsLayer::BULLET: return Layers::BULLET;
    default: return Layers::MOVING;
    }
}

/**
 * $BRIEF
 * this function builds a PhysicalRigidBody based on the provided shape type, position, and rotation.
 * \param shapeType
 * \param pos
 * \param rot
 * \return \b 
 */
PhysicalRigidBody PhysicsBodyBuilder::build(
    const glm::vec3& pos,
    const glm::quat& rot){

    if (!shape) {
        // Fallback to box if shape creation failed
        JPH::BoxShapeSettings fallback(JPH::Vec3(1.0f, 1.0f, 1.0f));
        shape = fallback.Create().Get();
    }
    BodyCreationSettings bodySettings(shape,
        toJPHVec3(pos),
        toJPHQuat(rot),
        motionType,
        layer);

    if(layer == 3)
    {
        bodySettings.mIsSensor = true;
    }

    if(lock_rotation)
    {
        bodySettings.mAllowedDOFs = EAllowedDOFs::TranslationX | EAllowedDOFs::TranslationY | EAllowedDOFs::TranslationZ;
    }

    //bodySettings.mFriction = 0;

    //BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
    //floor_shape_settings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

    //// Create the shape
    //ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
    //ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()
    //BodyCreationSettings floor_settings(shape, RVec3(0.0, -1.0, 0.0), Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);
    // Create the rigid body
    Body* createdBody = PhysicsManager::instance().mPhysicsSystem->GetBodyInterface().CreateBody(bodySettings);


    PhysicalRigidBody body(createdBody->GetID(), bodyInterface);

    return body;
}

PhysicalRigidBody PhysicsBodyBuilder::buildTemp(const JPH::Shape* shape,
    const glm::vec3& pos,
    const glm::quat& rot){

    if (!shape) {
        // Fallback to box if shape creation failed
        JPH::BoxShapeSettings fallback(JPH::Vec3(1.0f, 1.0f, 1.0f));
        shape = fallback.Create().Get();
    }
    BodyCreationSettings bodySettings(shape,
        toJPHVec3(pos),
        toJPHQuat(rot),
        motionType,
        toJoltLayer(static_cast<PhysicsLayer>(layer)));



    // Create the rigid body
    auto createdBody = bodyInterface.CreateBody(bodySettings);



    PhysicalRigidBody body(createdBody->GetID(), bodyInterface);

    reset(); // reset builder state after building

    return body;
}


void PhysicsBodyBuilder::reset()
{
    float mass = 1.0f;
    float friction = 0.5f;
    bool isSensor = false;
    JPH::EMotionType motionType = JPH::EMotionType::Dynamic;
    JPH::ObjectLayer layer = 0;
}
