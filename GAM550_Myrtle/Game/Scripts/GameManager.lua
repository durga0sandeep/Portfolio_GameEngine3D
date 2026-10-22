GameManager = {
    current_level = nil,
    current_prototype = "menu",  -- Track which prototype is active

    intro = "Game/Scripts/intro.lua",

    -- Separate level arrays for each prototype
    prototypes = {
        menu = {
            levels = {
                logoScreen = "Game/Scripts/UIMenu/LogoScreen.lua",
                mainMenu = "Game/Scripts/UIMenu/MainMenu.lua"
            },
            level_order = {"logoScreen", "mainMenu"}
        },
        prototype1 = {
            levels = {
                --level1 = "Game/Scripts/Levels/p3level1.lua",
                level1 = "Game/Scripts/Levels/Prototype1/level1.lua",
                level2 = "Game/Scripts/Levels/Prototype1/level2.lua",
                level3 = "Game/Scripts/Levels/Prototype1/level3.lua"
            },
            level_order = {"level1", "level2", "level3"}
        },
        prototype2 = {
            levels = {
                p2level1 = "Game/Scripts/Levels/Prototype2/p2level1.lua"
            },
            level_order = {"p2level1"}
        },
        prototype3 = {
            levels = {
                p3level1 = "Game/Scripts/Levels/p3level1.lua"
            },
            level_order = {"p3level1"}-- something
        }
    },
    
    level_needs_loading = false,
    level_needs_setup = false,
    level_completed_flag = false,  -- NEW: Prevent multiple triggers
    game_state = "MENU"  -- MENU, PLAYING, PAUSED, GAME_OVER
}

-----------------------------------------------------------------------------------

-----------------------------------------------------------------------------------

function GameManager:init() -- adds the scenes from the levels list
    print ("lua: init of game manager is called")
    -- Add scenes from all prototypes
    for prototypeName, prototypeData in pairs(self.prototypes) do
        for name, path in pairs(prototypeData.levels) do
            SceneManager:addScene(path)
        end
    end

     -- Start with first level of current prototype
    local firstLevel = self.prototypes[self.current_prototype].level_order[1]
    self.current_level = firstLevel
    self.level_needs_loading = true

    print("lua: Starting prototype:", self.current_prototype)
    print("lua: level:", self.current_level)
    print("lua: level needs loading:", tostring(self.level_needs_loading))

end
-----------------------------------------------------------------------------------

function GameManager:switchPrototype(prototypeName)
    if not self.prototypes[prototypeName] then
        print("lua: Prototype not found:", prototypeName)
        return false
    end
    
    print("lua: Switching to prototype:", prototypeName)
    self.current_prototype = prototypeName
    
    -- Load first level of new prototype
    local firstLevel = self.prototypes[prototypeName].level_order[1]
    self.current_level = firstLevel
    self.level_needs_loading = true
    self.level_completed_flag = false  -- Reset flag
    
    return true
end

-----------------------------------------------------------------------------------

function GameManager:forceLoadLevel(prototypeName, levelName)
    -- Validate prototype exists
    if not self.prototypes[prototypeName] then
        print("lua: Prototype not found:", prototypeName)
        return false
    end
    
    -- Validate level exists in prototype
    local levels = self.prototypes[prototypeName].levels
    if not levels[levelName] then
        print("lua: Level not found:", levelName, "in prototype:", prototypeName)
        return false
    end
    
    print("lua: Force loading level:", levelName, "from prototype:", prototypeName)
    
    -- Switch prototype if different
    if self.current_prototype ~= prototypeName then
        self.current_prototype = prototypeName
    end
    
    -- Load the specific level
    self.current_level = levelName
    self.level_needs_loading = true
    self.level_completed_flag = false
    
    return true
end

-----------------------------------------------------------------------------------

function GameManager:returnToMenu()
    print("lua: Returning to main menu")
    return self:forceLoadLevel("menu", "mainMenu")
end

-----------------------------------------------------------------------------------

function GameManager:getCurrentLevels()
    return self.prototypes[self.current_prototype].levels
end

-----------------------------------------------------------------------------------

function GameManager:loadLevel(levelName)
    local levels = self:getCurrentLevels()
    local path = levels[levelName]
    if not path then
        print("lua: Level not found:", levelName)
        return
    end

    print("lua: Unloading current scene")
    SceneManager:unloadCurrentScene()
    sky=nil
    char=nil
    ground=nil
    box=nil
    collectgarbage("collect")

    print("lua: level will be loaded")
    SceneManager:loadScene(path)
    self.current_level = levelName
    self.level_needs_loading = false
    self.level_needs_setup = true
    self.level_completed_flag = false  -- Reset flag for new level
end
-----------------------------------------------------------------------------------

function GameManager:setupLevel()
    if not self.current_level then
        return
    end

    local levels = self:getCurrentLevels()
    local path = levels[self.current_level]
    local currentScene = SceneManager:getSceneByPath(path)
    
    if not currentScene then
        print("lua: Failed to get scene")
        return
    end
    
    print("lua: Setting up level:", self.current_level)
    local success, levelSetup = pcall(dofile, path)
    if success and type(levelSetup) == "function" then
        levelSetup(currentScene)
        print("lua: Level setup complete")
    else
        print("lua: Error in level setup:", levelSetup)
    end
    
    self.level_needs_setup = false
end
-----------------------------------------------------------------------------------

function GameManager:determineNextLevel()
    local prototypeData = self.prototypes[self.current_prototype]
    local levelOrder = prototypeData.level_order
    local currentIndex = nil
    
    -- Find current level index
    for i, levelName in ipairs(levelOrder) do
        if levelName == self.current_level then
            currentIndex = i
            break
        end
    end
    
    if not currentIndex then
        print("lua: Warning - Current level not found in level_order")
        return nil
    end
    
    print("lua: Current level index:", currentIndex, "out of", #levelOrder)
    
    -- Return next level if exists
    if currentIndex < #levelOrder then
        local nextLevel = levelOrder[currentIndex + 1]
        print("lua: Next level determined:", nextLevel)
        return nextLevel
    end
    
    print("lua: No next level - at end of prototype")
    return nil
end

-----------------------------------------------------------------------------------

function GameManager:update(dt)
    -- Load level if flagged
    if self.level_needs_loading and self.current_level then
        print("lua: self needs loading")
        self:loadLevel(self.current_level)
    end

    if self.level_needs_setup then
        print("lua: setting up level")
        self:setupLevel()
    end

    -- Update current scene (always run)
    SceneManager:runScene()

    -- Check for forced level change from scene (only if not currently loading)
    if not self.level_needs_loading and not self.level_needs_setup then
        local levels = self:getCurrentLevels()
        local levelPath = levels[self.current_level]
        if levelPath then
            local currentScene = SceneManager:getSceneByPath(levelPath)
            if currentScene then
                -- Check if scene wants to change prototype
                if currentScene.getChangeLevel then
                    local changePrototype = currentScene:getChangeLevel()
                    
                    if changePrototype and changePrototype ~= "" then
                        print("lua: Level change detected - switching to prototype:", changePrototype)
                        self:switchPrototype(changePrototype)
                        -- Clear the change level flag in scene
                        if currentScene.clearChangeLevel then
                            currentScene:clearChangeLevel()
                        end
                    end
                end
            end
        end
    end

    -- Check for level completion (only if not already flagged and not loading)
    if self.current_level and not self.level_completed_flag and not self.level_needs_loading and not self.level_needs_setup then
        local levels = self:getCurrentLevels()
        local levelPath = levels[self.current_level]
        if levelPath then
            local currentScene = SceneManager:getSceneByPath(levelPath)
            if currentScene and currentScene:isLevelCompleted() then
                print("lua: Level completed detected!")
                self.level_completed_flag = true  -- Prevent multiple triggers
                
                local nextLevel = self:determineNextLevel()
                if nextLevel then
                    print("lua: Loading next level:", nextLevel)
                    self.current_level = nextLevel
                    self.level_needs_loading = true
                else
                    print("lua: No next level - looping back to start")
                    -- Loop back to first level of current prototype
                    local firstLevel = self.prototypes[self.current_prototype].level_order[1]
                    self.current_level = firstLevel
                    self.level_needs_loading = true
                end
            end
        end
    end
end
