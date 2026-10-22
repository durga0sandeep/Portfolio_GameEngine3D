return function(scene)
    print("LEVEL1: Setting up")
    local createSky = require("Game.Scripts.GameObjects.skySphere")
   --local createChar = require("Game.Scripts.GameObjects.SkeletalChar")
    --local createGround = require("Game.Scripts.GameObjects.ground")
    --local createBox = require("Game.Scripts.GameObjects.box_dup")

    print("Loading everything")

    local sky  = createSky(scene)
    --local char = createChar(scene)
    --local ground = createGround(scene)
     -- Generate 1000 boxes at random positions
   -- for i = 1, 100 do
     --   local x = math.random(-100, 100)
     ----   local y = math.random(100, 200)
     --  local z = math.random(-100, 100)
      --  local position = { x = x, y = y, z = z }
  --    --  createBox(scene, i, position)
   -- end

    --print("LEVEL2: Waiting for all assets to load...")
    --AssetLoadManager:waitForAllPendingLoads()
    print("LEVEL2: All assets loaded!")
    -- Add other level1 objects...
end


-----------------------------------------------------
--For level completion:
--from any game object: game object :setLevelCompleted(true)

--or since the scene is being sent:
--scene:setLevelCompleted(true)
