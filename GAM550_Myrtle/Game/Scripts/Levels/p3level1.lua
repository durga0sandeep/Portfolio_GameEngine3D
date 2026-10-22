return function(scene)
    print("LEVEL1: Setting up")
    local createSky = require("Game.Scripts.GameObjects.Prototype1.Shared.skySphere")
    local createChar = require("Game.Scripts.GameObjects.Prototype1.Shared.SkeletalChar")
    local createLevel = require("Game.Scripts.GameObjects.Prototype3.Level1.levelStructure")
    --local createUI = require("Game.Scripts.GameObjects.UI")
    local createLever = require("Game.Scripts.GameObjects.Prototype3.Level1.lever_one")
    local createPlatform = require("Game.Scripts.GameObjects.Prototype3.platform")
--    local createButton = require("Game.Scripts.GameObjects.Prototype1.Shared.button")
   -- local createDoor = require("Game.Scripts.GameObjects.Prototype1.Shared.door")
    --local createBGM = require("Game.Scripts.GameObjects.BackgroundMusic")
    local createSpotlightPlatform = require("Game.Scripts.GameObjects.Prototype3.Level1.SpotLightPlatform_one")
    local createDeathCollider = require("Game.Scripts.GameObjects.Prototype3.Level1.deathCollider")
    local createFlickeringPlatform = require("Game.Scripts.GameObjects.Prototype3.Level1.flickeringPlatform")
    local createSequentialPlatforms = require("Game.Scripts.GameObjects.Prototype3.Level1.sequentialPlatform")
    local createLeverTwo = require("Game.Scripts.GameObjects.Prototype3.Level1.lever_two")
    local createLeverThree = require("Game.Scripts.GameObjects.Prototype3.Level1.lever_three")
    local createDoor = require("Game.Scripts.GameObjects.Prototype3.Level1.door")
     local createSetting = require("Game.Scripts.UIMenu.SettingMenu")

    print("Loading everything")

    --local bgm = createBGM(scene)
    local sky  = createSky(scene)
    local levelStructure = createLevel(scene)
    local char = createChar(scene, 0, 10, -250)
    --local box = createBox(scene)
    --local ui = createUI(scene)
    local platform_one = createPlatform(scene, 0, 0, -200)
    local SpotLightPlatform_one = createSpotlightPlatform(scene, 0, 0, -150)
    local flickeringPlatform = createFlickeringPlatform(scene, 0, 0, -100)
    --local flickeringPlatform = createFlickeringPlatform(scene, 0, 0)
    local platform_two = createPlatform(scene, 0, 0, -50)
    local platform_three = createPlatform(scene, 0, 0, 165)
    local sequentialPlatform = createSequentialPlatforms(scene, 0, 0, 0)
    local door = createDoor(scene, 0, -3, 295, true)
    local SettingMenu = createSetting(scene)

    local leverOne = createLever(scene, -10, 2, -200)
    local leverTwo = createLeverTwo(scene, -10, 2, -50)
    local leverThree = createLeverThree(scene, -10, 2, 160)


  --  local door = createDoor(scene, 96, -3, 50, true)
--    local button = createButton(scene, 30, -4, 50)
    local deathCol = createDeathCollider(scene)
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
