return function(scene, x_val, y_val, z_val)
    print("GAME OBJECT: Creating auto-deactivating lever")
    local lever = scene:addGameObject("lever")
    local leverHandle = scene:addGameObject("leverHandle")
    local body_mesh
    local handle_mesh
    local phy
    local trigger
    local currentRotation = 0.785  -- Start at 45 degrees (0.785 radians)
    local targetRotation = -0.785
    local animationSpeed = 3.0
    local isAnimating = false
    local shouldReturn = false  -- Flag to trigger return animation
    local x_pos = x_val
    local y_pos = y_val
    local z_pos = z_val
    local sfx
    
    lever:setLuaStart(function(self)
        handle_mesh = leverHandle:addComponent("StaticMeshComponent", "Game/Assets/Models/Lever/leverhandle.obj", "Game/Assets/Textures/colormap.png")
        body_mesh = lever:addComponent("StaticMeshComponent", "Game/Assets/Models/Lever/lever.obj", "Game/Assets/Textures/colormap.png")
        phy = lever:addComponent("PhysicsComponent", "static", "static", 5, 5, 2)
        trigger = lever:addComponent("TriggerComponent", "sequential_platforms")
        sfx = lever:addComponent("AudioComponent", "Game/Assets/clank1-91862.mp3", "switched", true)

        phy:setPosition(x_pos, y_pos, z_pos)
        lever:setScale(20, 20, 20)
        lever:setPosition(x_pos, y_pos, z_pos)
        
        leverHandle:setPosition(x_pos, y_pos, z_pos)
        leverHandle:setScale(20, 20, 20)
        leverHandle:setRotation(0, 0, 0.785)
        
        lever:setLuaTrigger(function(self)
            print("Auto-deactivating lever triggered!")
            if not isAnimating then
                activate_Lever()
                trigger:action()
            end
        end)
        leverHandle:start()
    end)
    
    function activate_Lever()
        targetRotation = -0.785  -- Activate (down position)
        isAnimating = true
        shouldReturn = false
        sfx:playSound()
    end
    
    lever:setLuaUpdate(function(self, dt)
        if isAnimating then
            if math.abs(currentRotation - targetRotation) > 0.01 then
                currentRotation = currentRotation + (targetRotation - currentRotation) * animationSpeed * dt
                leverHandle:setRotation(0, 0, currentRotation)
            else
                -- Animation complete
                currentRotation = targetRotation  -- Snap to target
                leverHandle:setRotation(0, 0, currentRotation)
                
                if not shouldReturn then
                    -- Just finished activating, now return
                    print("Lever activation complete - starting return")
                    shouldReturn = true
                    targetRotation = 0.785  -- Return to up position
                else
                    -- Just finished returning
                    print("Lever return complete")
                    isAnimating = false
                    shouldReturn = false
                end
            end
        end
    end)
    
    lever:start()
    return lever
end
