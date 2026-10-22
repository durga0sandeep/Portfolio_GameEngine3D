/*!****************************************************************************
 * file  : SceneManager.h
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include <vector>
#include "Entities/Scene.h"
#include <sol/sol.hpp>
#include "LuaManager/LuaManager.h"

#pragma once
class SceneManager
{
private:
    SceneManager();
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;
    ~SceneManager() {};

    std::vector<std::unique_ptr<Scene>>     scenes;
    std::unordered_map<std::string, int>    scenePathToIndex;
    int                                     current_index;
    int                                     scene_count;
    std::vector<int>                        freeLists;

public:
    static  SceneManager& instance();
    void    loadScene(const std::string& scene_path);
    void    runScene();
    Scene*  addScene(const std::string& scene_path);
    Scene*  getSceneByPath(const std::string& scene_path);
    int     getCurrentScene();
    void    removeScene(int index);
    void    bindToLua(sol::state& lua);
    void    unloadCurrentScene();
    // should not expose to lua
};

