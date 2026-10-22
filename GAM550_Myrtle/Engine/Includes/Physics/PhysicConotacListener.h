#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <mutex>
#include <queue>

struct ContactEvent {
    JPH::BodyID body1;
    JPH::BodyID body2;
    JPH::ContactManifold manifold;
};

class PhysicsManager;
class PhysicContactListener : public JPH::ContactListener {
public:
    PhysicContactListener(PhysicsManager* manager);
       

    void OnContactAdded(const JPH::Body& body1, const JPH::Body& body2,
        const JPH::ContactManifold& manifold,
        JPH::ContactSettings& settings) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_contactQueue.push({ body1.GetID(), body2.GetID(), manifold });
    }

    // Call this at the end of your simulation step
    void ProcessQueuedContacts();

private:
    PhysicsManager* m_manager;
    std::mutex m_mutex;
    std::queue<ContactEvent> m_contactQueue;
};
