/*!****************************************************************************
 * file  : SceneGraph.h
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once

#include <Graphics/Light.h>
#include "Entities/Node.h"
#include "Entities/GameObject.h"

//class GameObject;
class SceneGraph : public Node// inherit from node
{
    /*
        constructor that sets the transform

        set parent to null ptr

        whenever a gameobject is created, it is set to be a child of scenegraph if it
        doesnt have any other parent.
    */
public:
    SceneGraph(std::string name) :Node(name) {};
    ~SceneGraph();

    void    addLight(const Light& light);
    const   std::vector<Light>& getLights() const;
    void    clear() { gameObjects.clear(); }
    GameObject* findGameObjectByName(const std::string& name);
    void    insertObject(GameObject* obj)
    {
        gameObjects.push_back(obj);
    }

private:
    // should handle by monolithic allocator
    std::vector<GameObject*>    gameObjects;
    std::vector<Light>          lights;
};
