return function(scene)
    print("LEVEL1: Setting up")
    local createSky = require("Game.Scripts.GameObjects.Prototype1.Shared.skySphere")
    local createChar = require("Game.Scripts.GameObjects.Prototype1.Shared.SkeletalChar")
    local createLevel = require("Game.Scripts.GameObjects.Prototype1.Level2.levelStructure")
    --local createBox = require("Game.Scripts.GameObjects.box")
    --local createUI = require("Game.Scripts.GameObjects.UI")
    local createLever = require("Game.Scripts.GameObjects.Prototype1.Shared.lever")
    local createHangBox = require("Game.Scripts.GameObjects.Prototype1.Shared.box_suspend")
    local createButton = require("Game.Scripts.GameObjects.Prototype1.Shared.button")
    local createDoor = require("Game.Scripts.GameObjects.Prototype1.Shared.door")
    local createSetting = require("Game.Scripts.UIMenu.SettingMenu")
    --local createBGM = require("Game.Scripts.GameObjects.BackgroundMusic")
    print("Loading everything")

    local sky  = createSky(scene)
    local levelStructure = createLevel(scene)
    local char = createChar(scene, -50, 50 , 0)
    --local ui = createUI(scene)
    local hangBox = createHangBox(scene, 0, 40, -40)
    local lever = createLever(scene, 0, -4, 0)
    local door = createDoor(scene, 96, 15, 50, false)
    --print("LEVEL1: Waiting for all assets to load...")
    --AssetLoadManager:waitForAllPendingLoads()
    local SettingMenu = createSetting(scene)
    print("LEVEL1: All assets loaded!")
    collectgarbage("collect")
end


-----------------------------------------------------
--For level completion:
--from any game object: game object :setLevelCompleted(true)

--or since the scene is being sent:
--scene:setLevelCompleted(true)
