/*!****************************************************************************
 * file  : SceneGraph.cpp
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include "pch.h"
#include "Entities\SceneGraph.h"

SceneGraph::~SceneGraph()
{
    gameObjects.clear();
}

void SceneGraph::addLight(const Light& light)
{
}

const std::vector<Light>& SceneGraph::getLights() const
{
    // TODO: insert return statement here
    return lights;
}

GameObject* SceneGraph::findGameObjectByName(const std::string& name)
{
    // Search in the gameObjects vector
    for (auto* gameObj : gameObjects)
    {
        if (gameObj->getName() == name)
            return gameObj;
    }

    //// If not found in the vector, search recursively through children
    //// (in case some GameObjects are nested)
    //for (auto* child : children)
    //{
    //    GameObject* gameObj = dynamic_cast<GameObject*>(child);
    //    if (gameObj && gameObj->getName() == name)
    //        return gameObj;

    //    // Recursively search in child's children
    //    if (auto* foundObj = findGameObjectRecursive(child, name))
    //        return foundObj;
    //}

    return nullptr;
}
