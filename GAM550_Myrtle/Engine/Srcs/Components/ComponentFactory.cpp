/*!****************************************************************************
 * file  : ComponentFactory.cpp
 * brief : Implementatin of component factory class
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/


#include "pch.h"
#include "Components\ComponentFactory.h"
#include "Components\Component.h"
/**
 * $BRIEF   Allows the association of a string with a component
 *          this will simplify how lua script will be written for
 *          adding a specific component.
 * 
 * \param name          component name
 * \param creator       the component
 * \param required_args the number of required arguments for the creation of
                        the component
 */
void ComponentFactory::registerComponent(const std::string& name, ComponentCreator creator, size_t required_args, ComponentDowncaster downcaster)
{
    creators[name] = { creator, downcaster, required_args };
}


/**
 * $BRIEF       How the factory creates a component from a string
 * 
 * \param name  the name of the component
 * \param owner the gameobject that owns that component
 * \param args  (variadic list) of arguments associated with this component
 * \return \b   a pointer to the newly created component
 */
std::shared_ptr<Component> ComponentFactory::createComponent(const std::string& name, GameObject* owner, sol::variadic_args args)
{
    auto it = creators.find(name);
    if (it != creators.end())
    {
        if (args.size() != it->second.required_args)
        {
            std::cerr << "Error: component '" << name << "' requries "
                << it->second.required_args << "arguments.\n";
        }
        return it->second.component(owner, args);
    }
    std::cerr << "Error: component '" << name << "' is not registered.\n";
    return nullptr;
}

sol::object ComponentFactory::createComponentForLua(const std::string& name, GameObject* owner, sol::variadic_args args, std::shared_ptr<Component>& outComponent)
{
    auto it = creators.find(name);
    if (it != creators.end())
    {
        auto comp = it->second.component(owner, args);
        outComponent = comp;

        if (comp && it->second.downcaster)
        {
            return it->second.downcaster(args.lua_state(), comp);
        }
        comp = nullptr;
        int i = outComponent.use_count();
        // Fallback to base Component
        return sol::make_object(args.lua_state(), comp);
    }
    outComponent = nullptr;
    return sol::lua_nil;
}
