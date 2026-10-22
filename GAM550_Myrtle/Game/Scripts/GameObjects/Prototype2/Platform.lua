return function(scene, x, y, z)
    print("GAME OBJECT: Creating disappearing platform")
    local disappear_Platform = scene:addGameObject("Platform")
    local mesh_component
    local phy
    local particles
    local sfx
    
    -- State machine variables (declared here, initialized in start)
    local state
    local timer
    local triggerTime
    local blinkDuration
    local disappearDuration
    local boxSizeX
    local boxSizeY
    
    -- Set collision callback AFTER component is created
    disappear_Platform:setLuaStart(function(self)
        print("GAME OBJECT: Start - Creating disappear_Platform components")

        mesh_component = disappear_Platform:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/floor-small-square.obj", 
            "Game/Assets/Textures/colormap.png")

        phy = disappear_Platform:addComponent("PhysicsComponent", "static", "non_moving", 20, 0.1, 20)
        particles = disappear_Platform:addComponent("ParticleComponent", "Texture/smoke_01.png", 100, false)
        --sfx = disappear_Platform:addComponent("AudioComponent", "..\\assets\\audio\\SFX_box\\footstep_carpet_000.ogg", "collide2", true)
        
        disappear_Platform:setScale(40, 0.1, 40)
        phy:setGravityEnabled(false)
        phy:setPosition(x, y, z)
        
        particles:setEmissionRate(50)  -- 50 particles per second
        particles:setAutoEmit(false)   -- Manually controlled emission
        particles:setFollowOwner(true)
        
        -- Initialize state machine
        state = "idle"
        timer = 0
        triggerTime = math.random(0, 2)
        blinkDuration = 2
        disappearDuration = 1
        boxSizeX = 40
        boxSizeY = 0.1
        
        
    end)

        
    disappear_Platform:start()
    return disappear_Platform
end
