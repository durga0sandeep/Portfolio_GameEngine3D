/*!****************************************************************************
 * file  : GameObject.h
 * brief : GameObject - The GameObject class is the base class for GameObject, It is a Node
 * It needs a name in order to find it under the game object tree
 * need to Update on each game loop
 * Also have a list of Component that will update each tick
 * 
 * 
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once

#include "Node.h"
#include <memory>

class Scene;
class Component;
/**
 * @class GameObject
 * @brief Represents an entity in the scene graph that can hold multiple components.
 *
 * GameObject inherits from Node and acts as a container for Components, enabling
 * modular behavior and rendering logic. Components can be added, retrieved, and
 * updated dynamically. Typically, each GameObject corresponds to a visible or
 * interactive object in the game world.
 */
class GameObject : public Node
{
public:
    GameObject(const std::string name);
    ~GameObject();

    void update() override;
    void start();
    void trigger();
    /**
    * @brief Adds a component to the GameObject.
    *
    * @tparam T The type of the component.
    * @param component A shared pointer to the component to be added.
    *
    * Should call the set owner of every component (if applicable)
    */
    template <typename T>
    std::shared_ptr<T> AddComponent(std::shared_ptr<T> component)
    {
        ComponentList.push_back(component);
        return component;  // Return the component that was added
    }

    /**
    * @brief Finds a component in the GameObject.
    *
    * @tparam T The type of the component to find.
    * @return A shared pointer to the component if found, otherwise nullptr.
    */
    template <typename T>
    std::shared_ptr<T> FindComponent()
    {
        for (const auto& component : ComponentList)
        {
            // Use dynamic_pointer_cast to check if the component is of type T
            auto castedComponent = std::dynamic_pointer_cast<T>(component);
            if (castedComponent)
            {
                return castedComponent;
            }
        }
        // Return nullptr if no component of type T is found
        return nullptr;
    }
    static void bindToLua(sol::state& lua);
    void setLuaUpdate(sol::function func);
    void setLuaStart(sol::function func);
    void setLuaTrigger(sol::function func);
    void cleanup();
    Scene* getOwnerScene() const { return ownerScene; }
    void setOwnerScene(Scene* scene) { ownerScene = scene; }

private:
    //CollisionListener* collisionListener;
    std::vector<std::shared_ptr<Component>> ComponentList = std::vector<std::shared_ptr<Component>>();
    sol::function luaUpdateFunc;
    sol::function luaStartFunc;
    sol::function luaTriggerFunc;

    Scene* ownerScene = nullptr;
};
