/*!****************************************************************************
 * file  : scene.cpp
 * brief :
 *
 * author: Salam Sibai
 * Course: CS550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include "pch.h"
#include "Entities/Scene.h"
#include <Physics/PhysicsManager.h>
#include <Graphics/Renderer.h>
#include <UI/UIManager.h>

Scene::Scene(int idx, const std::string &path) : index(idx), scene_path(path), level_completed(false), scene_graph(SceneGraph("SceneGraphRoot" + std::to_string(idx))),
                                                 allocator(MonolithicAllocator<GameObject>(1500))
{
}

int Scene::getIndex()
{
    return index;
}

SceneGraph &Scene::getSceneGraph()
{
    return scene_graph;
}

// void    Scene::setupScene()
//{
//     initializeSceneGraph();
// }

GameObject *Scene::addGameObject(const std::string &name)
{
    GameObject *obj = allocator.allocate(name);
    // adds to scenegraph
    obj->setOwnerScene(this);
    scene_graph.addChild(obj);
    scene_graph.insertObject(obj);
    return obj;
}

// void    Scene::initializeSceneGraph()
//{
//     read the file
//     initialize the scene graph based on this file
//     call update of that scenegraph.
// }

void Scene::update()
{
    scene_graph.update();
}

/**
 * $BRIEF Binds adding game object to lua. Must bind all functions
 *
 * \param lua
 */
void Scene::bindToLua(sol::state &lua)
{
    lua.new_usertype<Scene>("Scene",
                            "addGameObject", &Scene::addGameObject,
                            "isLevelCompleted", &Scene::isLevelCompleted,
                            "setLevelCompleted", &Scene::setLevelCompleted,
                            "setChangeLevel", &Scene::setChangeLevel,
                            "clearChangeLevel", &Scene::clearChangeLevel,
                            "getChangeLevel", &Scene::getChangeLevel);
}

void Scene::setLevelCompleted(bool level_status)
{
    level_completed = level_status;
}

bool Scene::isLevelCompleted()
{
    return level_completed;
}

void Scene::clearScene()
{
    std::cout << "Clearing scene: " << scene_path << std::endl;
    scene_graph.clear();
    allocator.reset();
    PhysicsManager::instance().reset();
    Renderer::instance().getRenderGraph().getRenderQueue()->reset();
    UIManager::instance().clear();
    Renderer::instance().getSpotLightList().reset();
    Renderer::instance().getLocalLightList().reset();
    level_completed = false;
}

void Scene::setChangeLevel(const std::string &proname)
{
    changeLevel = proname;
}

std::string Scene::getChangeLevel()
{
    return changeLevel;
}

void Scene::clearChangeLevel()
{
    changeLevel = "";
}
