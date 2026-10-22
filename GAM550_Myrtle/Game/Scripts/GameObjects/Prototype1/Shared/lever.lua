return function(scene, x_val, y_val, z_val)
    --print("GAME OBJECT: Creating lever")
    local lever = scene:addGameObject("lever")
    local leverHandle = scene:addGameObject("leverHandle")
    local body_mesh
    local handle_mesh
    local phy
    local trigger
    local isActivated = false
    local currentRotation = 0.785  -- Start at 45 degrees (0.785 radians)
    local targetRotation = -0.785
    local animationSpeed = 3.0
    local isAnimating = false
    local done = false
    local x_pos = x_val
    local y_pos = y_val
    local z_pos = z_val
    local sfx
    
    lever:setLuaStart(function(self)
        handle_mesh = leverHandle:addComponent("StaticMeshComponent", "Game/Assets/Models/Lever/leverhandle.obj", "Game/Assets/Textures/colormap.png")
        body_mesh = lever:addComponent("StaticMeshComponent", "Game/Assets/Models/Lever/lever.obj", "Game/Assets/Textures/colormap.png")
        phy = lever:addComponent("PhysicsComponent", "static", "static", 5, 5, 2)
        trigger = lever:addComponent("TriggerComponent", "hanging_box")
        sfx = lever:addComponent("AudioComponent", "Game/Assets/clank1-91862.mp3", "switched", true)

        phy:setPosition(x_pos, y_pos, z_pos)
        lever:setScale(20, 20, 20)
        lever:setPosition(x_pos, y_pos, z_pos)
        
        leverHandle:setPosition(x_pos, y_pos, z_pos)
        leverHandle:setScale(20, 20, 20)
        leverHandle:setRotation(0, 0, 0.785)
        
        lever:setLuaTrigger(function(self)
            --print("Lever trigger called!")
            activateLever()
            if not done then
                done = true
                trigger:action()
            end
        end)
        leverHandle:start()
    end)
    
    function activateLever()
        isActivated = not isActivated
        targetRotation = isActivated and -0.785 or 0.785
        isAnimating = true
        sfx:playSound()
    end
    
    lever:setLuaUpdate(function(self, dt)
        -- Only animate if isAnimating flag is true
        if isAnimating then
            if math.abs(currentRotation - targetRotation) > 0.01 then
                currentRotation = currentRotation + (targetRotation - currentRotation) * animationSpeed * dt
                leverHandle:setRotation(0, 0, currentRotation)  -- Rotate around Z axis
            else
                -- Animation complete
                isAnimating = false
                currentRotation = targetRotation  -- Snap to target
                leverHandle:setRotation(0, 0, currentRotation)
                --print("Lever animation complete")
            end
        end
    end)
    
    lever:start()
    return lever
end
