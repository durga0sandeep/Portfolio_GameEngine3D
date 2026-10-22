/*!****************************************************************************
 * file  : LuaManager.cpp
 * brief :
 *
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include "pch.h"
#include "LuaManager\LuaManager.h"
#include "Components\ComponentList.h"
#include "Entities\SceneManager.h"
#include "Entities\Scene.h"
#include "Entities\GameObject.h"
#include "Audio\AudioManager.h"
#include <Components/UIComponent.h>
#include <Components/BoxSuspender.h>
#include <Components/ScoreSystemComponent.h>
#include <Components/InputComponenet.h>
#include <Components/SpotLightComponent.h>

void LuaManager::initialize()
{
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package);
    bindToAll(getState());

    try
    {
        lua.script_file("Game/Scripts/GameManager.lua");
        lua.script("GameManager:init()");
    }
    catch (const sol::error &e)
    {
        std::cerr << "[Lua Error in initialize]: " << e.what() << std::endl;
    }
}

void LuaManager::runScript(const std::string &path)
{
    try
    {
        sol::load_result script = lua.load_file(path);
        sol::protected_function_result result = script();
        // lua.script_file(path);
    }
    catch (const sol::error &e)
    {
        std::cerr << "[Lua Error]: " << e.what() << std::endl;
    }
}

void LuaManager::bindToAll(sol::state &lua)
{
    bindCommonTypes(lua);
    SceneManager::instance().bindToLua(lua);
    Scene::bindToLua(lua);
    GameObject::bindToLua(lua);
    AssetLoadManager::bindToLua(lua);
    AudioManager::instance().bindToLua(lua);
    bindComponents(lua);
}

void LuaManager::bindComponents(sol::state &lua)
{
    SkySphereComponent::registerWithFactory();
    SkeletalMeshComponent::registerWithFactory(lua);
    PlayerController::registerWithFactory(lua);
    StaticMeshComponent::registerWithFactory(lua);
    PhysicsComponent::registerWithFactory(lua);
    ParticleComponent::registerWithFactory(lua);
    // UIComponent::registerWithFactory(lua);
    AudioComponent::registerWithFactory(lua);
    TriggerComponent::registerWithFactory(lua);
    BoxSuspender::registerWithFactory(lua);
    ScoreSystemComponent::registerWithFactory(lua);
    UITextComponent::registerWithFactory(lua);
    UIButtonComponent::registerWithFactory(lua);
    UIPanelComponent::registerWithFactory(lua);
    InputComponenet::registerWithFactory(lua);
    LightComponent::registerWithFactory(lua);
    SpotLightComponent::registerWithFactory(lua);
}

void LuaManager::update(float delta_time)
{
    try
    {
        sol::table gameManager = lua["GameManager"];
        sol::protected_function updateFunc = gameManager["update"];
        if (updateFunc.valid())
        {
            sol::protected_function_result result = updateFunc(gameManager, delta_time); // Pass gameManager as self
            if (!result.valid())
            {
                sol::error err = result;
                std::cerr << "Lua update error: " << err.what() << std::endl;
            }
        }
    }
    catch (const sol::error &e)
    {
        std::cerr << "[Lua Error in update]: " << e.what() << std::endl;
    }
}

/**
 * @brief Registers common types used across Lua scripts
 *
 * This function registers frequently used C++ types to be accessible in Lua.
 * Currently registers glm::vec3 as both a usertype (for instances) and a
 * factory function (for construction).
 *
 * Registration includes:
 * - vec3_type: The usertype defining vec3 instance properties (x, y, z fields)
 * - vec3(): A global factory function that constructs vec3 objects
 *   Usage in Lua: local pos = vec3(x, y, z)
 *
 * Note: The factory function approach is used instead of making the usertype
 * callable because sol2's new_usertype creates type definitions for instances,
 * not constructor tables. Registering vec3 as a separate function allows
 * natural constructor syntax: vec3(1, 2, 3)
 *
 * @param lua The sol2 state to register types into
 */
void LuaManager::bindCommonTypes(sol::state &lua)
{
    lua.new_usertype<glm::vec3>("vec3_type",
                                "x", &glm::vec3::x,
                                "y", &glm::vec3::y,
                                "z", &glm::vec3::z);

    // Register vec3 as a FACTORY FUNCTION
    lua.set_function("vec3", sol::overload(
                                 []()
                                 {
                                     // std::cout << "Creating vec3 with no args" << std::endl;
                                     return glm::vec3();
                                 },
                                 [](float x, float y, float z)
                                 {
                                     // std::cout << "Creating vec3 with: " << x << ", " << y << ", " << z << std::endl;
                                     return glm::vec3(x, y, z);
                                 }));

    // Register quat
    lua.new_usertype<glm::quat>("quat_type",
                                "x", &glm::quat::x,
                                "y", &glm::quat::y,
                                "z", &glm::quat::z,
                                "w", &glm::quat::w);

    lua.set_function("quat", sol::overload(
                                 []()
                                 {
                                     std::cout << "Creating quat with no args" << std::endl;
                                     return glm::quat();
                                 },
                                 [](float x, float y, float z, float w)
                                 {
                                     std::cout << "Creating quat with: " << x << ", " << y << ", " << z << ", " << w << std::endl;
                                     return glm::quat(w, x, y, z); // glm::quat's constructor takes (w, x, y, z)
                                 }));
    std::cout << "vec3 registration complete!" << std::endl;
}
