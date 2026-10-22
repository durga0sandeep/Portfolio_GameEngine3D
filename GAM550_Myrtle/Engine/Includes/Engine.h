/*!****************************************************************************
 * file  : Engine.h
 * brief :
 *
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once

#include "pch.h"
#include "Graphics/Fbo.h"
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Physics/PhysicsManager.h"
#include "Memory/AssetLoadManager.h"
#include "Entities/SceneManager.h"
#include "FRController/FramerateController.h"
#include "Input/Input.h"
#include "Audio/AudioManager.h"
#include <Graphics/SkeletalAnimation/mySkeleton.h>
#include <Graphics/SkeletalAnimation/SkeletonAnimator.h>
#include <Graphics/SkeletalAnimation/SkeletonMesh.h>
#include "LuaManager/LuaManager.h"
#include <UI/UIText.h>

// static FramerateController FRC;

class Engine
{
private:
    Engine();
    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;
    Engine(Engine &&) = delete;
    Engine &operator=(Engine &&) = delete;
    ~Engine() {};

    std::unique_ptr<GameWindow> window;

    Renderer &renderer;
    PhysicsManager &physics_manager;
    AssetLoadManager &asset_manager;
    FramerateController &framerate_controller;
    Input &input_manager;
    SceneManager &scene_manager;
    LuaManager &lua_manager;
    AudioManager &audio_manager;

    UIText *framerateText;
    bool isUIVisible = true;


    // temp only, replace by scene graph
    std::shared_ptr<Shader> Vertex;
    std::vector<PhysicalRigidBody> Bodies;
    //std::vector<AudioManager> audio;
    AudioManager& audio;
    
    AssetLoadManager assetMgr;

    
public:
    static Engine &instance();
    void runEngine();
    glm::vec2 getWindowSize();
};
