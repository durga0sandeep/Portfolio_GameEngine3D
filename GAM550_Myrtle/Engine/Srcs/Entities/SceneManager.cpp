/*!****************************************************************************
 * file  : SceneManager.cpp
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include "pch.h"
#include "Entities\SceneManager.h"
#include "Memory/AssetLoadManager.h"

SceneManager::SceneManager()
    : current_index(0), scene_count(0) {}

SceneManager& SceneManager::instance()
{
    static SceneManager instance;
    return instance;
}

Scene* SceneManager::getSceneByPath(const std::string& scene_path)
{
    auto it = scenePathToIndex.find(scene_path);
    if (it != scenePathToIndex.end())
    {
        int index = it->second;
        if (index >= 0 && index < scenes.size())
        {
            return scenes[index].get();
        }
    }
    return nullptr;
}

/**
 * $BRIEF            sets up which scene will be loaded
 * 
 * \param scene_path the path to the scene to load
 */
void SceneManager::loadScene(const std::string& scene_path)
{
    std::cout << "Load scene " << scene_path << " being called\n";
    auto it = scenePathToIndex.find(scene_path);
    if(it == scenePathToIndex.end())
    {
        std::cerr << "Scene not found: " << scene_path << "\n";
        return;
    }
    int index = it->second;
    if (index >= scenes.size() || !scenes[index])
    {
        std::cerr << "Scene index invalid for path: " << scene_path << "\n";
        return;
    }
    current_index = index;
    std::cout << "current index: " << index << "\n";
}

void SceneManager::runScene()
{
    scenes[current_index]->update();
}

/**
 * $BRIEF   Adds scene
 * 
 *          a unique ptr is being created first of the scene
 *          so that this scene would be constructed with its
 *          correct index.
 * 
 */
Scene* SceneManager::addScene(const std::string& scene_path)
{
    std::cout << "add scene is called\n";
    int index = static_cast<int>(scenes.size());
    auto scene = std::make_unique<Scene>(index, scene_path);
    Scene* scene_ptr = scene.get();
    scenePathToIndex[scene_path] = index;
    scenes.push_back(std::move(scene));
    scene_count = static_cast<int>(scenes.size());
    return scene_ptr;
}

void SceneManager::removeScene(int index)
{
    scenes[index]->getAllocator().reset();
}

void SceneManager::bindToLua(sol::state& lua)
{
    lua.new_usertype<SceneManager>("SceneManager",
        "addScene", &SceneManager::addScene,
        "loadScene", &SceneManager::loadScene,
        "runScene", & SceneManager::runScene,
        "getSceneByPath", & SceneManager::getSceneByPath,
        "unloadCurrentScene", & SceneManager::unloadCurrentScene);
    lua["SceneManager"] = std::ref(SceneManager::instance());
}

void SceneManager::unloadCurrentScene()
{
    if (current_index >= 0 && current_index < scenes.size() && scenes[current_index])
    {
        std::cout << "Unloading scene at index: " << current_index << std::endl;
        scenes[current_index]->clearScene();
    }
}

int SceneManager::getCurrentScene()
{
    return current_index;
}
