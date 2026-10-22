/*!****************************************************************************
 * file  : LuaManager.h
 * brief :  LuaManager class - has all basic lua calls +
 *          binds all needed functions to lua.
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once

#include <sol/sol.hpp>
#include <string>

class LuaManager
{
public:
    void initialize();
    void runScript(const std::string& path);
    sol::state& getState() { return lua; }
    void bindToAll(sol::state& lua);
    void bindComponents(sol::state& lua);
    static LuaManager& instance()
    {
        static LuaManager manager;
        return manager;
    }
    void    loadAllScripts() {} //loads all lua scripts in a directory
    void    update(float delta_time);
    void    bindCommonTypes(sol::state& lua);
    ~LuaManager() {};
private:
    sol::state lua;
    LuaManager() {};
    LuaManager(const LuaManager&) = delete;
    LuaManager& operator=(const LuaManager&) = delete;
    LuaManager(LuaManager&&) = delete;
    LuaManager& operator=(LuaManager&&) = delete;
};
