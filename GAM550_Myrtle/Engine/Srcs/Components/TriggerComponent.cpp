#include "pch.h"
#include "Components/TriggerComponent.h"
#include "Components/ComponentFactory.h"
#include "Entities/Scene.h"
#include "Entities/SceneGraph.h"

TriggerComponent::TriggerComponent(GameObject* _owner, std::string _other) : other(nullptr)
{
    setOwner(_owner);
    if (_other != "")
        setOther(_other);
}

void TriggerComponent::action()
{
    assert(other != nullptr);
    //std::cout << "will call other trigger " << other->getName() << "\n";
    other->trigger();
}

void TriggerComponent::setOther(const std::string& _other)
{
    std::cout << "Setting other gameobject by name: " << _other << "\n";
    other = owner->getOwnerScene()->getSceneGraph().findGameObjectByName(_other);
    if (other) {
        std::cout << "Found object: " << other->getName() << " at address: " << other << "\n";
    }
    else {
        std::cout << "ERROR: Could not find object with name: " << _other << "\n";
    }
}

void TriggerComponent::setOther(PhysicalRigidBody* other_body)
{
    if (other_body && other_body->getOwnerComponent())
        other = other_body->getOwnerComponent()->getOwner();
}

void TriggerComponent::setOther(GameObject* _other)
{
    other = _other;
}

void TriggerComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent
    ("TriggerComponent",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {
            std::string otherName = "";
            if (args.size() > 0)
                otherName = args[0];
            return std::make_shared<TriggerComponent>(owner, otherName);
        },
        0,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<TriggerComponent>(comp);
            return sol::make_object(L, derived);
        }
    );
    bindToLua(lua);
}

void TriggerComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<TriggerComponent>("TriggerComponent",
        sol::base_classes, sol::bases<Component>(),
        "action", &TriggerComponent::action,
        "setOther", sol::overload(
            static_cast<void(TriggerComponent::*)(GameObject*)>(&TriggerComponent::setOther),
            static_cast<void(TriggerComponent::*)(const std::string&)>(&TriggerComponent::setOther),
            static_cast<void(TriggerComponent::*)(PhysicalRigidBody*)>(&TriggerComponent::setOther)
        )
    );
}

