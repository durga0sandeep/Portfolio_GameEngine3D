return function(scene)
    print("LEVEL1: Setting up")
    local createSky = require("Game.Scripts.GameObjects.Prototype1.Shared.skySphere")
    local createChar = require("Game.Scripts.GameObjects.Prototype2.SkeletalCharPro2")
    local createLevel = require("Game.Scripts.GameObjects.Prototype2.Level1.levelStructure")
    local createLightBlub = require("Game.Scripts.GameObjects.Prototype2.CollectiveLight")
    local createLightBlubred = require("Game.Scripts.GameObjects.Prototype2.CollectiveLightred")
    local createLightBlubblue = require("Game.Scripts.GameObjects.Prototype2.CollectiveLightblue")
    local createDisappearPlateform = require("Game.Scripts.GameObjects.Prototype2.DisappearPlatform")
    local createPlateform = require("Game.Scripts.GameObjects.Prototype2.Platform")
    local createSetting = require("Game.Scripts.UIMenu.SettingMenu")

    print("Loading everything")

    --local bgm = createBGM(scene)
    local sky  = createSky(scene)
    local levelStructure = createLevel(scene)
    local char = createChar(scene, 0, 2, 0)
    local lightbulbblue = createLightBlubblue(scene)
    local lightbulbred = createLightBlubred(scene)
    local staticPlatform = createPlateform(scene, 0, 1, 0)
    local platformCount = 0
    for i = 45, -45, -45 do  -- step by -10 from 90 down to 0
        for j = 45, -45, -45 do  -- step by -10 from 90 down to 0
            if(i == 0 and j == 0) then
                --skip platform where player starts
            else
                createDisappearPlateform(scene, i, 1, j)
                platformCount = platformCount + 1
            end
        end
    end
    local SettingMenu = createSetting(scene)

    print("LEVEL1: All assets loaded!")
    collectgarbage("collect")
end


-----------------------------------------------------
--For level completion:
--from any game object: game object :setLevelCompleted(true)

--or since the scene is being sent:
--scene:setLevelCompleted(true)
