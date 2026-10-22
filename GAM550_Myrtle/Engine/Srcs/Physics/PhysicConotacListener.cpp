#include "pch.h"
#include "Physics/PhysicConotacListener.h"
#include "Physics/PhysicsManager.h"
PhysicContactListener::PhysicContactListener(PhysicsManager* manager):m_manager(manager)
{
}

void PhysicContactListener::ProcessQueuedContacts()
{
    std::queue<ContactEvent> localQueue;

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::swap(localQueue, m_contactQueue);
    }

    while (!localQueue.empty()) {
        const ContactEvent& event = localQueue.front();
        m_manager->HandleContact(event.body1, event.body2, event.manifold);
        localQueue.pop();
    }
}
