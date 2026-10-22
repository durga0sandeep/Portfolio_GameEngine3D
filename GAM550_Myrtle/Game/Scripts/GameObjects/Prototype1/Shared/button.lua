return function(scene, x_val, y_val, z_val)
    --print("GAME OBJECT: Creating button")
    local button_base = scene:addGameObject("button_base")
    local button = scene:addGameObject("button")
    local base_mesh
    local button_mesh
    local phy
    local sfx
    local is_activated = false
    local is_animating = false
    local animation_speed = 3.0
    
    -- Base position (frame)
    local base_x = x_val
    local base_y = y_val
    local base_z = z_val
    
    -- Button starts 2.5 units higher than base (on top of base)
    local button_y_offset = 2.5
    local button_y_start = base_y + button_y_offset
    local button_y_pressed = base_y  -- Button presses down to base level
    
    local current_y = button_y_start
    local target_y = button_y_start
    
    local other_obj = nil
    local interaction_distance = 10.0
    local was_button_pressed = false
    local trigger
    
    button:setLuaStart(function(self)
        base_mesh = button_base:addComponent("StaticMeshComponent", "Game/Assets/Models/button/buttonbase.obj", "Game/Assets/Textures/colormap.png")
        button_mesh = button:addComponent("StaticMeshComponent", "Game/Assets/Models/button/button.obj", "Game/Assets/Textures/colormap.png")
        phy = button:addComponent("PhysicsComponent", "static", "trigger", 5, 0.5, 5)
        trigger = button:addComponent("TriggerComponent", "door")
        sfx = button:addComponent("AudioComponent", "Game/Assets/correct.mp3", "pressed", true)

        -- Position button at starting height
        phy:setPosition(base_x, current_y, base_z)
        button:setScale(30, 30, 30)
        
        -- Position base
        button_base:setPosition(base_x, base_y, base_z)
        button_base:setScale(30, 20, 30)
        
        phy:setOnCollision(function(self, contactPoint, normal, depth, otherComponent)
            if otherComponent and other_obj == nil then
                other_obj = otherComponent:getOwnerObject()
                print("Object detected: " .. other_obj:getName())
                sfx:playSound()
            end
        end)
    end)
    
    function activateButton(pressed)
        if is_activated ~= pressed then
            --print("Button state changing to: " .. tostring(pressed))
            is_activated = pressed
            -- When pressed, go to base level; when released, go to start height
            target_y = is_activated and button_y_pressed or button_y_start
            is_animating = true
        end
    end
    
    button:setLuaUpdate(function(self, dt)
        -- Check distance if we have an object reference
        if other_obj ~= nil then
            local other_pos = other_obj:getPosition()
            local button_pos = button:getPosition()
            
            -- Calculate distance
            local dx = other_pos.x - button_pos.x
            local dy = other_pos.y - button_pos.y
            local dz = other_pos.z - button_pos.z
            local distance = math.sqrt(dx*dx + dy*dy + dz*dz)
            
            -- Check if object is on button (close enough)
            local is_on_button = distance <= interaction_distance
            
            -- Detect state changes
            if is_on_button and not was_button_pressed then
                --print("Object stepped on button!")
                trigger:action()
                activateButton(true)
            elseif not is_on_button and was_button_pressed then
                --print("Object left button!")
                trigger:action()
                activateButton(false)
            end
            
            was_button_pressed = is_on_button
            
            -- Clear object reference if they're far away
            if distance > interaction_distance * 2 then
                other_obj = nil
                was_button_pressed = false
            end
        end
        
        -- Animate button position along Y-axis
        if is_animating then
            if math.abs(current_y - target_y) > 0.01 then
                current_y = current_y + (target_y - current_y) * animation_speed * dt
                phy:setPosition(base_x, current_y, base_z)
            else
                -- Animation complete
                is_animating = false
                current_y = target_y
                phy:setPosition(base_x, current_y, base_z)
                --print("Button animation complete at Y: " .. current_y)
            end
        end
    end)
    
    button:start()
    return button
end
