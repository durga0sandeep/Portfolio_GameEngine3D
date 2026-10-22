return function(scene)
    print("LEVEL1: Setting up")
    local createSky = require("Game.Scripts.GameObjects.Prototype1.Shared.skySphere")
    local createChar = require("Game.Scripts.GameObjects.Prototype1.Shared.SkeletalChar")
    local createLevel = require("Game.Scripts.GameObjects.Prototype1.Level1.levelStructure")
    --local createBox = require("Game.Scripts.GameObjects.box")
    --local createUI = require("Game.Scripts.GameObjects.UI")
    local createLever = require("Game.Scripts.GameObjects.Prototype1.Shared.lever")
    local createHangBox = require("Game.Scripts.GameObjects.Prototype1.Shared.box_suspend")
    local createButton = require("Game.Scripts.GameObjects.Prototype1.Shared.button")
    local createDoor = require("Game.Scripts.GameObjects.Prototype1.Shared.door")
    --local createLogoScreen = require("Game.Scripts.UIMenu.LogoScreen")
    --local createMainMenu = require("Game.Scripts.UIMenu.SettingMenu")
    local createSetting = require("Game.Scripts.UIMenu.SettingMenu")
    --local createBGM = require("Game.Scripts.GameObjects.BackgroundMusic")
    print("Loading everything")

    --local bgm = createBGM(scene)
    local sky  = createSky(scene)
    local levelStructure = createLevel(scene)
    local char = createChar(scene, -45, 50, -45)
    --local box = createBox(scene)
    --local ui = createUI(scene)
    local hangBox = createHangBox(scene, 0, 20, -40)
    local lever = createLever(scene, 0, -4, 0)
    local door = createDoor(scene, 96, -3, 50, true)
    local button = createButton(scene, 30, -4, 50)
    local SettingMenu = createSetting(scene)
    --local UILogoScreen = createLogoScreen(scene)
    --local MainMenu = createMainMenu(scene)
    --print("LEVEL1: Waiting for all assets to load...")
    --AssetLoadManager:waitForAllPendingLoads()
    print("LEVEL1: All assets loaded!")
    collectgarbage("collect")
end


-----------------------------------------------------
--For level completion:
--from any game object: game object :setLevelCompleted(true)

--or since the scene is being sent:
--scene:setLevelCompleted(true)
