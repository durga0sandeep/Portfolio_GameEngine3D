return function(scene, x_val, y_val, z_val, locked)
    print("GAME OBJECT: Creating door")
    local door = scene:addGameObject("door")
    local door_frame = scene:addGameObject("door_frame")
    local door_mesh
    local frame_mesh
    local phy
    local door_opened = false
    local open_amount = 11  -- How far the door slides
    local is_animating = false
    local animationSpeed = 3.0
    local frame_z_pos = z_val-- 50
    local y_pos = y_val -- -3
    local z_pos = frame_z_pos - 3  -- Base Z position (closed state)
    local current_z = z_pos  -- Track current Z position
    local target_z = z_pos   -- Target Z position
    local x_pos = x_val -- 96
    
    door:setLuaStart(function(self)
        frame_mesh = door_frame:addComponent("StaticMeshComponent", "Game/Assets/Models/Door/wall-doorway-sliding.obj", "Game/Assets/Textures/colormap.png")
        door_frame:setScale(20, 20, 20)
        phy = door_frame:addComponent("PhysicsComponent", "static", "static", 2, 5, 10)
        phy:setPosition(x_pos, y_pos, frame_z_pos)
        
        door_mesh = door:addComponent("StaticMeshComponent", "Game/Assets/Models/Door/door-sliding.obj", "Game/Assets/Textures/colormap.png")
        door:setPosition(x_pos, y_pos, current_z)
        door:setScale(20, 20, 20)

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
        target_z = door_opened and (z_pos - open_amount) or z_pos
        is_animating = true
    end
    
    door:setLuaUpdate(function(self, dt)
        -- Animate door sliding along Z-axis
        if is_animating then
            if math.abs(current_z - target_z) > 0.1 then
                current_z = current_z + (target_z - current_z) * animationSpeed * dt
                door:setPosition(x_pos, y_pos, current_z)
            else
                -- Animation complete
                is_animating = false
                current_z = target_z  -- Snap to target
                door:setPosition(x_pos, y_pos, current_z)
                print("Door animation complete at Z: " .. current_z)
            end
        end
    end)
    
    door_frame:start()
    door:start()
    return door
end
