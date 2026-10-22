/*!****************************************************************************
 * file  : Scene.h
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once
#include "SceneGraph.h"
#include "Camera.h"
#include "Memory/MonolithicAllocator.h"
#include <sol/sol.hpp>

class Scene
{
    /*

     Control to lights
     Control to sound manager to change bg music
     SceneGraph obj
     Container of cameras

     init scenegraph

    */
private:
    SceneGraph  scene_graph;
    int         index;
    std::string scene_path;
    bool        level_completed;
    std::vector<std::unique_ptr<Camera>>            cameras;
    MonolithicAllocator<GameObject>                 allocator;
 //   void        initializeSceneGraph();
    std::string changeLevel;
    //string      changeLevelName;

public:
    Scene(int   idx, const std::string& scene_path);
    int         getIndex();
    SceneGraph& getSceneGraph();
    //void        setupScene();
    GameObject* addGameObject(const std::string& name);
    void        update();
    static void bindToLua(sol::state& lua);
    void        setLevelCompleted(bool level_status);
    bool        isLevelCompleted();
    void        clearScene();
    void        setChangeLevel(const std::string& proname);
    std::string getChangeLevel();
    void        clearChangeLevel();
    MonolithicAllocator<GameObject>& getAllocator() { return allocator; }

};
