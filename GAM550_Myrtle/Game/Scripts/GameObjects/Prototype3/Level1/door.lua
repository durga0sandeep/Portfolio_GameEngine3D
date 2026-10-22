return function(scene, x_val, y_val, z_val, locked)
    print("GAME OBJECT: Creating door")
    local door = scene:addGameObject("door")
    local door_frame = scene:addGameObject("door_frame")
    local door_phy = scene:addGameObject("door_phy")
    local phy
    local door_mesh
    local frame_mesh
    local door_opened = false
    local open_amount = 11  -- How far the door slides
    local is_animating = false
    local animationSpeed = 3.0
    local frame_z_pos = z_val
    local y_pos = y_val
    local z_pos = z_val
    local x_pos = x_val  -- Base X position (closed state)
    local current_x = x_pos - 3  -- Track current X position
    local target_x = x_pos   -- Target X position
    
    door:setLuaStart(function(self)
        frame_mesh = door_frame:addComponent("StaticMeshComponent", "Game/Assets/Models/Door/wall-doorway-sliding.obj", "Game/Assets/Textures/colormap.png")
        door_frame:setScale(20, 20, 20)
        door_frame:setRotation(0, 3.14*2.5, 0)
        door_frame:setPosition(x_pos, y_pos, frame_z_pos)
        
        phy = door_phy:addComponent("PhysicsComponent", "static", "static", 5, 2, 10)
        phy:setPosition(x_pos, y_pos, z_pos)
        
        door_mesh = door:addComponent("StaticMeshComponent", "Game/Assets/Models/Door/door-sliding.obj", "Game/Assets/Textures/colormap.png")
        door:setPosition(current_x, y_pos, z_pos)
        door:setScale(20, 20, 20)
        door:setRotation(0, 3.14*2.5, 0)
        
        if not locked then
            toggleDoor()
        end
        
        door:setLuaTrigger(function(self)
            print("Trigger door called")
            toggleDoor()
        end)
        
        phy:setOnCollision(function(self, contactPoint, normal, depth, otherComponent)
            if door_opened then
                local otherGameObject = otherComponent:getOwnerObject()
                if otherGameObject:getName() == "Player" then
                    print("level ended!")
                    scene:setLevelCompleted(true)
                end
            else
                print("Door not yet opened")
            end
        end)
    end)
    
    function toggleDoor()
        door_opened = not door_opened
        print("Door state changing to: " .. tostring(door_opened))
        
        -- Calculate target: if open, subtract open_amount; if closed, use base position
        target_x = door_opened and (x_pos - open_amount) or x_pos
        is_animating = true
    end
    
    door:setLuaUpdate(function(self, dt)
        -- Animate door sliding along X-axis
        if is_animating then
            if math.abs(current_x - target_x) > 0.1 then
                current_x = current_x + (target_x - current_x) * animationSpeed * dt
                door:setPosition(current_x, y_pos, z_pos)
            else
                -- Animation complete
                is_animating = false
                current_x = target_x  -- Snap to target
                door:setPosition(current_x, y_pos, z_pos)
                print("Door animation complete at X: " .. current_x)
            end
        end
    end)
    
    door_frame:start()
    door:start()
    return door
end
