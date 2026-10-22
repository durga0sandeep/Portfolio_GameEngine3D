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
    
    -- Blinking variables
    local blink_timer = 0
    local blink_interval = 0.5  -- Starting interval (slow)
    local is_visible = true
    
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
        
        print("Platform initialized in state: " .. state .. " with trigger time: " .. triggerTime)
    end)

    disappear_Platform:setLuaUpdate(function(self, dt)
        timer = timer + dt
        if state == "idle" then
            -- Wait until random trigger time
            if timer >= triggerTime then
                print("Platform: IDLE -> BLINKING (timer: " .. timer .. " >= " .. triggerTime .. ")")
                state = "blinking"
                timer = 0
                blink_timer = 0
                is_visible = true
            end

        elseif state == "blinking" then
            -- Calculate blink interval based on time
            -- 0-1 seconds: slow blinking (0.4 second interval)
            -- 1-2 seconds: fast blinking (accelerates from 0.4 to 0.05 seconds)
            if timer < 1.0 then
                blink_interval = 0.4  -- Slow, steady blink for first half
            else
                -- Fast accelerating blink for second half
                local progress = (timer - 1.0) / 1.0  -- Progress from 0 to 1 in the second half
                blink_interval = 0.4 - (progress * 0.35)  -- Goes from 0.4 to 0.05
            end
            
            -- Handle blinking effect
            blink_timer = blink_timer + dt
            
            -- Toggle visibility at intervals
            if blink_timer >= blink_interval then
                blink_timer = 0
                is_visible = not is_visible
                
                -- Toggle visibility
                if mesh_component.setIsVisible then
                    mesh_component:setIsVisible(is_visible)
                elseif disappear_Platform.setIsVisible then
                    disappear_Platform:setIsVisible(is_visible)
                end
            end
            
            -- Emit particles in box area
            local px = phy:getPosition().x
            local py = phy:getPosition().y
            local pz = phy:getPosition().z
            
            -- Emit particles randomly inside the box
            for i = 1, 5 do -- emit 5 particles per frame during blink
                local offsetX = (math.random() - 0.5) * boxSizeX
                local offsetY = (math.random() - 0.5) * boxSizeY
                local offsetZ = (math.random() - 0.5) * boxSizeX
                
                particles:setEmitterPosition(px + offsetX, py + offsetY, pz + offsetZ)
                particles:emit()
            end
            
            if timer >= blinkDuration then
                print("Platform: BLINKING -> DISAPPEAR")
                state = "disappear"
                timer = 0
                phy:setPosition(px, py - 100, pz) -- drop floor down
                
                -- Make sure platform is invisible when it disappears
                if mesh_component.setIsVisible then
                    mesh_component:setIsVisible(false)
                elseif disappear_Platform.setIsVisible then
                    disappear_Platform:setIsVisible(false)
                end
            end

        elseif state == "disappear" then
            -- Stay disappeared for 1 second
            if timer >= disappearDuration then
                print("Platform: DISAPPEAR -> RESET")
                local px = phy:getPosition().x
                local py = phy:getPosition().y
                local pz = phy:getPosition().z
                phy:setPosition(px, py + 100, pz) -- reset floor back up
                
                -- Make sure platform is visible again
                if mesh_component.setIsVisible then
                    mesh_component:setIsVisible(true)
                elseif disappear_Platform.setIsVisible then
                    disappear_Platform:setIsVisible(true)
                end
                is_visible = true
                
                state = "reset"
                timer = 0
            end

        elseif state == "reset" then
            -- Reset back to idle with new random trigger
            print("Platform: RESET -> IDLE")
            state = "idle"
            timer = 0
            triggerTime = math.random(5, 10)
            print("New trigger time: " .. triggerTime)
        end
    end)
    
    disappear_Platform:start()
    return disappear_Platform
end
