return function(scene)
    print("LEVEL3: Setting up")
    local createSky = require("Game.Scripts.GameObjects.Prototype1.Shared.skySphere")
    local createChar = require("Game.Scripts.GameObjects.Prototype1.Shared.SkeletalChar")
    local createLevel = require("Game.Scripts.GameObjects.Prototype1.Level3.levelStructure")
    local createBox = require("Game.Scripts.GameObjects.Prototype1.Shared.box")
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
    local hangBox = createHangBox(scene, -50, 30, 40)
    local lever = createLever(scene, 80, 15, 20)
    local door = createDoor(scene, 96, 15, 50, true)
    local button = createButton(scene, -40, -4, -40)
    local box = createBox(scene)
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
