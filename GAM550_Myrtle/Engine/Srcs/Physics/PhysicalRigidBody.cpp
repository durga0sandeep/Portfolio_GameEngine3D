#include "pch.h"
#include "Physics\PhysicalRigidBody.h"
#include "Physics\PhysicsManager.h"
#include "Physics\PhysicsBodyBuilder.h"
#include "Components\PhysicsComponent.h"

/**
 * $BRIEF
 * sets the position of the rigid body in world space.
 * \param pos
 * \param act
 */
void PhysicalRigidBody::setPosition(const glm::vec3& pos, ActivationMode act)
{
    mBodyInterface.SetPosition(mID,toJPHVec3(pos), static_cast<JPH::EActivation>(static_cast<int>(act)));
}

/**
 * $BRIEF
 * sets the rotation of the rigid body in world space.
 * \param rot
 * \param act
 */
void PhysicalRigidBody::setRotation(const glm::quat& rot, ActivationMode act)
{
    mBodyInterface.SetRotation(mID, toJPHQuat(rot), static_cast<JPH::EActivation>(static_cast<int>(act)));
}

/**
 * $BRIEF
 * sets the position and rotation of the rigid body in world space.
 * \param pos
 * \param rot
 * \param act
 */
void PhysicalRigidBody::setTransform(const glm::vec3& pos, const glm::quat& rot, ActivationMode act)
{
    mBodyInterface.SetPositionAndRotation(mID, toJPHVec3(pos), toJPHQuat(rot), static_cast<JPH::EActivation>(static_cast<int>(act)));
}

/**
 * $BRIEF
 * gets the position of the rigid body in world space.
 * \return \b 
 */
glm::vec3 PhysicalRigidBody::getPosition() const
{
    return toGLMVec3(mBodyInterface.GetPosition(mID));
}

/**
 * $BRIEF
 * gets the rotation of the rigid body in world space.
 * \return \b 
 */
glm::quat PhysicalRigidBody::getRotation() const{
    return toGLMQuat(mBodyInterface.GetRotation(mID));
}

/**
 * $BRIEF
 * gets the transform of the rigid body in world space.
 * \return \b 
 */
glm::mat4 PhysicalRigidBody::getWorldTransform() const{
    return toGLMMat4(mBodyInterface.GetWorldTransform(mID));
}

// Velocity & Forces

/**
 * $BRIEF
 * sets the linear velocity of the rigid body in world space.
 * \param vel
 */
void PhysicalRigidBody::setLinearVelocity(const glm::vec3& vel) {
    mBodyInterface.SetLinearVelocity(mID, JPH::Vec3(vel.x, vel.y, vel.z));
}

/**
 * $BRIEF
 * sets the angular velocity of the rigid body in world space.
 * \param vel
 */
void PhysicalRigidBody::setAngularVelocity(const glm::vec3& vel) {
    mBodyInterface.SetAngularVelocity(mID, JPH::Vec3(vel.x, vel.y, vel.z));
}

/**
 * $BRIEF
 * adds a force to the rigid body in world space.
 * \param force
 */
void PhysicalRigidBody::addForce(const glm::vec3& force) {
    mBodyInterface.AddForce(mID, JPH::Vec3(force.x, force.y, force.z));
}

/**
 * $BRIEF
 * adds a torque to the rigid body in world space.
 * \param torque
 */
void PhysicalRigidBody::addTorque(const glm::vec3& torque) {
    mBodyInterface.AddTorque(mID, JPH::Vec3(torque.x, torque.y, torque.z));
}

void PhysicalRigidBody::addImpulse(const glm::vec3& impulse)
{
    mBodyInterface.AddImpulse(mID, JPH::Vec3(impulse.x, impulse.y, impulse.z));
}

void PhysicalRigidBody::addImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& point)
{
    mBodyInterface.AddImpulse(mID, JPH::Vec3(impulse.x, impulse.y, impulse.z), JPH::Vec3(point.x, point.y, point.z));
}

void PhysicalRigidBody::addForceAtPoint(const glm::vec3& force, const glm::vec3& point)
{
    mBodyInterface.AddForce(mID, JPH::Vec3(force.x, force.y, force.z), JPH::Vec3(point.x, point.y, point.z));
}

/**
 * $BRIEF
 * getter for linear velocity of the rigid body in world space.
 * \return \b 
 */
glm::vec3 PhysicalRigidBody::getLinearVelocity() const {
    JPH::Vec3 v = mBodyInterface.GetLinearVelocity(mID);
    return glm::vec3(v.GetX(), v.GetY(), v.GetZ());
}

/**
 * $BRIEF
 * getter for angular velocity of the rigid body in world space.
 * \return \b 
 */
glm::vec3 PhysicalRigidBody::getAngularVelocity() const {
    JPH::Vec3 v = mBodyInterface.GetAngularVelocity(mID);
    return glm::vec3(v.GetX(), v.GetY(), v.GetZ());
}

// Activation & Sleep

/**
 * $BRIEF
 * activates the rigid body. make it respond to forces and collisions simulation
 */
void PhysicalRigidBody::activate() {
    //mBodyInterface.ActivateBody(mID);
    mBodyInterface.AddBody(mID, EActivation::Activate);
}

/**
 * $BRIEF
 * deactivates the rigid body. make it ignore forces and collisions simulation
 */
void PhysicalRigidBody::deactivate() {
    //mBodyInterface.DeactivateBody(mID);
    mBodyInterface.RemoveBody(mID);
}

/**
 * $BRIEF
 * checks if the rigid body is active.
 * \return \b 
 */
bool PhysicalRigidBody::isActive() const {
    return mBodyInterface.IsActive(mID);
}

/**
 * $BRIEF
 * checks if the rigid body is sleeping.
 * \return \b 
 */
void PhysicalRigidBody::resetSleepTimer() {
    mBodyInterface.ResetSleepTimer(mID);
}

// Properties & Metadata

/**
 * $BRIEF
 * gets the motion type of the rigid body.
 * all motion types are: Static, Kinematic, Dynamic.
 * \return \b 
 */
MotionType PhysicalRigidBody::getMotionType() const {
    return toMotionType(mBodyInterface.GetMotionType(mID));
}

/**
 * $BRIEF
 * sets the motion type of the rigid body.
 * all motion types are: Static, Kinematic, Dynamic.
 * \param type
 * \param act
 */
void PhysicalRigidBody::setMotionType(MotionType type, ActivationMode act)
{
    mBodyInterface.SetMotionType(mID, toJPHMotion(type), toJPHActivation(act));
}

/**
 * $BRIEF
 * user data getter and setter for the rigid body.
 * it bound to be a pointer to an mesh in the engine.
 * \return \b 
 */
uint64_t PhysicalRigidBody::getUserData() const {
    return mBodyInterface.GetUserData(mID);
}

/**
 * $BRIEF
 * user data setter for the rigid body.
 * it bound to be a pointer to an mesh in the engine.
 * \param data
 */
void PhysicalRigidBody::setUserData(uint64_t data) {
    mBodyInterface.SetUserData(mID, data);
}

// Shape & Collision
//const JPH::Shape* PhysicalRigidBody::getShape() const {
//    return mBodyInterface.getShape(mID);
//}

/**
 * $BRIEF
 * sets the shape of the rigid body.
 * internal use only.
 * shape should not be change after creation of the rigid body.
 * if want to change the shape, it should be removed and re-added to the physics system.
 * \param shape
 * \param updateMassProperties
 * \param act
 */
//void PhysicalRigidBody::setShape(const JPH::Shape* shape, bool updateMassProperties, ActivationMode act) {
//    mBodyInterface.SetShape(mID, shape, updateMassProperties, toJPHActivation(act));
//}

/**
 * $BRIEF
 * invalidates the contact cache of the rigid body.
 * internal use only.
 * should be called after changing the shape of the rigid body.
 */
void PhysicalRigidBody::invalidateContactCache() {
    mBodyInterface.InvalidateContactCache(mID);
}

//JPH::TransformedShape PhysicalRigidBody::getTransformedShape() const {
//    return mBodyInterface.getTransformedShape(mID);
//}

// Advanced Utilities
/**
 * $BRIEF
 * gets the velocity of a point on the rigid body in world space.
 * \param point
 * \return \b 
 */
glm::vec3 PhysicalRigidBody::getPointVelocity(const glm::vec3& point) const {
    JPH::Vec3 p(point.x, point.y, point.z);
    JPH::Vec3 v = mBodyInterface.GetPointVelocity(mID, p);
    return glm::vec3(v.GetX(), v.GetY(), v.GetZ());
}

/**
 * $BRIEF
 * gets the inertia tensor of the rigid body in world space.
 * not sure what it can used for. just put it here for in case.
 * \return \b 
 */
glm::mat4 PhysicalRigidBody::getInverseInertia() const {
    JPH::Mat44 m = mBodyInterface.GetInverseInertia(mID);
    return toGLMMat4(m);
}

/**
 * $BRIEF
 * 
 * \param info
 * \param other
 */
void PhysicalRigidBody::DispatchCollision(const CollisionInfo& info, PhysicalRigidBody* other)
{
    if (m_ownerComponent) {
        m_ownerComponent->OnCollision(info, other);
    }
}

void PhysicalRigidBody::setOwner(PhysicsComponent* _owner)
{
    m_ownerComponent = _owner;
}


/*
* to do:
 for the mesh bounds calculation, assuming you have a list of vertices and a scale factor:

JPH::Vec3 minBounds(FLT_MAX, FLT_MAX, FLT_MAX);
JPH::Vec3 maxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX);

for (const auto& vertex : meshVertices) {
    JPH::Vec3 scaled = vertex * scale; // Apply uniform or per-axis scale
    minBounds = minBounds.Min(scaled);
    maxBounds = maxBounds.Max(scaled);
}

JPH::AABox meshBounds(minBounds, maxBounds);


*/





