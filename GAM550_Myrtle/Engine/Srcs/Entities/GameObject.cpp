/*!****************************************************************************
 * file  : GameObject.cpp
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : November 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include "pch.h"
#include "Entities/GameObject.h"
#include "Components/Component.h"
#include "Components/ComponentFactory.h"
#include "FRController/FramerateController.h"
#include "Entities/Scene.h" 
/**
 * @brief
 *
 * Constructor for GameObject Base class, we only need to set the name for searching
 * no other logic is needed because it is only a base class
 *
 * @param name name of the GameObject
 * @return GameObject.
 */
GameObject::GameObject(const std::string name)
    :Node(name)
{}


/**
 * $BRIEF GameObject destructor
 * 
 * 
 * 
 *      This is throwing an exception 
 * 
 * 
 * void deref(lua_State* L_) const noexcept {
			luaL_unref(L_, LUA_REGISTRYINDEX, ref);
		}
 */
GameObject::~GameObject()
{
    cleanup();
}

void GameObject::cleanup()
{
    if (luaUpdateFunc.valid())
        luaUpdateFunc.abandon();
    if (luaStartFunc.valid())
        luaStartFunc.abandon();
    if (luaTriggerFunc.valid())
        luaTriggerFunc.abandon();
}

/**
 * @brief Update Loop GameObject
 *
 * @return no return
 */
void GameObject::update()
{
    if (luaUpdateFunc.valid())
        luaUpdateFunc(*this, FramerateController::instance().GetFrameTime()); // pass 'self' and deltaTime
    for (std::shared_ptr<Component>component : ComponentList)
    {
        int i = component.use_count();
        component->update();
    }
    Node::update();
}

void GameObject::trigger()
{
    std::cout << "trigger called\n";
    if (luaTriggerFunc.valid())
    {
        std::cout << "trigger function in other is valid\n";
        luaTriggerFunc(*this);
    }
}

void GameObject::start()
{
    //std::cout << "cpp start is called\n";
    if (luaStartFunc.valid())
        luaStartFunc(*this);
    //load stuff 
}

/**
 * $BRIEF   Binds functions to lua
 * 
 * lua["GameObject"]["addComponent"]
 *   This tells sol2 that you want to add a new member function called addComponent to the GameObject usertype in Lua
 * 
 * The lambda function: creates an instance of the componenet, and adds it to our gameobject
 * 
 * \param lua
 */
void GameObject::bindToLua(sol::state& lua)
{
    lua.new_usertype<GameObject>("GameObject",
        sol::constructors<GameObject(const std::string&)>(),
        "addComponent", [](GameObject& self, const std::string& componentName, sol::variadic_args args) -> sol::object
        {
            std::shared_ptr<Component> comp;
            sol::object luaObject = ComponentFactory::instance().createComponentForLua(componentName, &self, args, comp);

            if (comp)
            {
                self.AddComponent(comp);
                //return comp;
            }
            else
            {
                std::cout << "Failed to create component: " << componentName << std::endl;
                return nullptr;
            }
            return luaObject;
        },
        "setLuaUpdate", &GameObject::setLuaUpdate,
        "setLuaStart", &GameObject::setLuaStart,
        "setLuaTrigger", &GameObject::setLuaTrigger,
        "start", &GameObject::start,
        "Trigger", &GameObject::trigger,
        "setPosition", sol::overload(
            [](GameObject& self, glm::vec3 pos)
            {
                self.setLocalPosition(pos);
            },
            [](GameObject& self, float x, float y, float z)
            {
                self.setLocalPosition(glm::vec3(x, y, z));
            }
        ),
        "setScale", sol::overload(
            [](GameObject& self, glm::vec3 scale)
            {
                self.setLocalScale(scale);
            },
            [](GameObject& self, float x, float y, float z)
            {
                self.setLocalScale(glm::vec3(x, y, z));
            }
        ),
        "setRotation", sol::overload(
            [](GameObject& self, const glm::vec3& rot)
            {
                self.setLocalRotation(rot);
            },
            [](GameObject& self, float x, float y, float z)
            {
                self.setLocalRotation(glm::vec3(x, y, z));
            },
            [](GameObject& self, const glm::quat& rot)
            {
                self.setLocalRotation(rot);
            }
        ),
        "getPosition", [](GameObject& self) -> glm::vec3
        {
            return self.getLocalPosition();  // Returns by value
        },
        "getRotation", [](GameObject& self) -> glm::vec3
        {
            return self.getLocalRotation();
        },
        "getScale", [](GameObject& self) -> glm::vec3
        {
            return self.getLocalScale();
        },
        "getName", [](GameObject& self) -> std::string
        {
            return self.getName();  // Returns by value
        },
        "getOwnerScene", & GameObject::getOwnerScene,
        "setLevelCompleted", [](GameObject& self, bool status)
        {
            if (self.getOwnerScene())
            {
                self.getOwnerScene()->setLevelCompleted(status);
            }
        }
    );
}

void GameObject::setLuaUpdate(sol::function func)
{
    luaUpdateFunc = func;
}

void GameObject::setLuaStart(sol::function func)
{
    luaStartFunc = func;
}

void GameObject::setLuaTrigger(sol::function func)
{
    luaTriggerFunc = func;
}

/*
Lua code:
local sky = GameObject("Sky")
sky:addComponent("SkySphereComponent", "Model/Obj/sky_icoshpere.obj", "Texture/sky.jpg")

local player = GameObject("Player")
player:addComponent("PlayerController")
player:addComponent("PhysicsComponent", 10.0, 1.5)  -- any extra arguments the component expects

* */
