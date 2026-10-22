#include "pch.h"
#include "Components/BoxSuspender.h"
#include "Entities/GameObject.h"
#include "Components/ComponentFactory.h"

/**
 * $BRIEF
 * Attach back to some anchor point, incase we need this
 * \param anchorWorld
 * \param ropeLength
 */
void BoxSuspender::attachToSwingPoint(const glm::vec3& anchorWorld, float ropeLength)
{
    detach(); // Remove any existing constraint

    glm::vec3 ownerPos = ownerPhysics->getPosition();

    staticAnchor.setPosition(anchorWorld);

    // Create a fixed-length distance constraint
    constraint = PhysicsManager::instance().createDistanceConstraint(
        ownerPhysics->getBody()->getBodyID(),     // Player body
        staticAnchor.getBodyID(),                // Static anchor (world)
        ownerPos,                               // Anchor on player
        anchorWorld,                             // Anchor in world
        ropeLength,                              // Min distance
        ropeLength                               // Max distance
    );

    attachedBody = ownerPhysics->getBody()->getBodyID();
}

/**
 * $BRIEF
 * detach the object
 */
void BoxSuspender::detach()
{
    if (constraint) {
        PhysicsManager::instance().removeConstraint(constraint);
        constraint = nullptr;
        attachedBody = JPH::BodyID();
    }
}

/**
 * $BRIEF
 * Test only please remove
 */
void BoxSuspender::update()
{
    // Keyboard input
    if (Input::instance().IsKeyHeld(KeyCode::L)) {
        detach();
    }
    if (Input::instance().IsKeyHeld(KeyCode::K)) {
        attachToSwingPoint(glm::vec3(0,100,0),50);
    }
}

/**
 * $BRIEF
 * cheack if contrain(rope) exist
 * \return \b 
 */
bool BoxSuspender::isAttached() const
{
    return constraint != nullptr;
}

/**
 * $BRIEF
 * regester to lua
 * \param lua
 */
void BoxSuspender::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent(
        "BoxSuspender",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<BoxSuspender>
        {
            glm::vec3 anchorWorld;
            float distance;

            // Check if first arg is a vec3 or three separate floats
            if (args.size() >= 4 && args[0].is<float>())
            {
                // Three floats + distance
                anchorWorld = glm::vec3(args[0].as<float>(), args[1].as<float>(), args[2].as<float>());
                distance = args[3].as<float>();
            }
            else
            {
                // vec3 + distance
                anchorWorld = args[0].as<glm::vec3>();
                distance = args[1].as<float>();
            }

            return std::make_shared<BoxSuspender>(owner, anchorWorld, distance);
        },
        0,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<BoxSuspender>(comp);
            return sol::make_object(L, derived);
        }
    );
    bindToLua(lua);  // Don't forget to bind!
}

/**
 * $BRIEF
 * bind to lua
 * \param lua
 */
void BoxSuspender::bindToLua(sol::state& lua)
{
    lua.new_usertype<BoxSuspender>("BoxSuspender",
        sol::base_classes, sol::bases<Component>(),
        "isAttached", &BoxSuspender::isAttached,
        "detach", &BoxSuspender::detach,
        "attachToSwingPoint", &BoxSuspender::attachToSwingPoint
    );
}
