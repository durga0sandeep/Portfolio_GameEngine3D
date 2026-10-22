/*!****************************************************************************
 * file  : ComponentFactory.h
 * brief : Component factory - generic factory for creating components by 
 *                             string name, designed to work with Lua
 * 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once
#include <sol/sol.hpp>
#include "Component.h"
#include "ComponentList.h"
#include <unordered_map>
#include <functional>
#include "Entities/GameObject.h"

class ComponentFactory
{
public:

    ///////////////////////////////////////
    /**
    *  ComponentCreator is a function that takes the pointer to the gameobject
    *  that owns the component + any number of arguments from lua
    *  this makes adding any component modular and binding it with lua
    *  would be much easier.
    *
    * \return \b the newly created component.
    */
    using ComponentCreator = std::function<std::shared_ptr<Component>(GameObject*, sol::variadic_args)>;
    using ComponentDowncaster = std::function<sol::object(lua_State*, std::shared_ptr<Component>)>;

    static ComponentFactory& instance()
    {
        static ComponentFactory factory;
        return factory;
    }

    void registerComponent(const std::string& name, ComponentCreator creator, size_t required_args, ComponentDowncaster downcaster = nullptr);
    std::shared_ptr<Component> createComponent(const std::string& name, GameObject* owner, sol::variadic_args args);
    sol::object createComponentForLua(const std::string& name, GameObject* owner, sol::variadic_args args, std::shared_ptr<Component>& outComponent);
    ~ComponentFactory() {};

private:
    struct ComponentInfo
    {
        ComponentFactory::ComponentCreator component;
        ComponentDowncaster downcaster;
        size_t required_args;
    };
    std::unordered_map<std::string, ComponentInfo> creators;
    ComponentFactory() {};
};

