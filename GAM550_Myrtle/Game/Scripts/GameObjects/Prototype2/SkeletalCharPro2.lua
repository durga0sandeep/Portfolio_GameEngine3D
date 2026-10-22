return function(scene, x_val, y_val, z_val)
    print("GAME OBJECT: Creating skeleton")
    local char = scene:addGameObject("Player")
    local skeletal_mesh
    local phy
    local player_controller
    local trigger
    local score_component
    local within_range = false
    local other_gameobj = nil
    local interaction_distance = 10.0
    local correct_sfx
    local wrong_sfx
    
    -- Blinking variables
    local is_blinking = false
    local blink_timer = 0
    local blink_duration = 2.0  -- Total duration of blinking effect (2 seconds)
    local blink_interval = 0.2  -- Time between visibility toggles (0.2 seconds = visible for 0.2s, invisible for 0.2s)
    local blink_elapsed = 0
    local is_visible = true
    
    print("player controller added")
    
    --Set the Lua start function
    char:setLuaStart(function(self)
        char:setScale(0.1, 0.1, 0.1)
        skeletal_mesh = char:addComponent("SkeletalMeshComponent", "Game/Assets/AnimClips/Player/Idle.fbx")
        phy = char:addComponent("PhysicsComponent", "dynamic", "player", 2, 3.75, 2, true)
        correct_sfx = char:addComponent("AudioComponent", "Game/Assets/correct.mp3", "pressed", true)
        wrong_sfx = char:addComponent("AudioComponent", "Game/Assets/wrong.mp3", "pressed", true)

        player_controller = char:addComponent("PlayerController")
        trigger = char:addComponent("TriggerComponent")
        score_component = char:addComponent("ScoreSystemComponent")
        phy:setPosition(x_val, y_val, z_val)
        print("THE PLAYER IS SETUP")
        
        -- Configure movement
        print("starting to configure controller")
        player_controller:setMoveSpeed(20.0)
        player_controller:setRunSpeedMultiplier(2.0)
        player_controller:setJumpForce(1600000.0 * 4)
        print("movement set!")
        
        -- Configure camera
        player_controller:setCameraDistance(60.0)
        player_controller:setCameraSensitivity(0.15)
        print("camera set!")
        
        -- Load animations
        skeletal_mesh:addAnimClip("Game/Assets/AnimClips/Player/Idle.fbx", "idle")
        skeletal_mesh:addAnimClip("Game/Assets/AnimClips/Player/Walking.fbx", "walk")
        skeletal_mesh:addAnimClip("Game/Assets/AnimClips/Player/Running.fbx", "run")
        skeletal_mesh:addAnimClip("Model/Fbx/Jumping_Up.fbx", "jump")
        
        char:setLuaTrigger(function(self)
            print("TRIGGER CALLED!")
        end)
        
        phy:setOnCollision(function(self, contactPoint, normal, depth, otherComponent)
            local otherGameObject = otherComponent:getOwnerObject()
            if otherGameObject:getName() == "bluelight" then
                print("PLAYER TOUCHED bluelight!")
                if(score_component:getColor() == 1) then
                    score_component:updatePoint(1)
                    correct_sfx:playSound()
                else
                    score_component:updatePoint(-1)
                    wrong_sfx:playSound()
                end
                score_component:setColor(math.random(0,1))
            end
            if otherGameObject:getName() == "redlight" then
                print("PLAYER TOUCHED redlight!")
                if(score_component:getColor() == 0) then
                    score_component:updatePoint(1)
                    correct_sfx:playSound()

                else
                    score_component:updatePoint(-1)
                    wrong_sfx:playSound()
                end
                score_component:setColor(math.random(0,1))
            end
            print(score_component:getPoint())
        end)
    end)
    
    char:setLuaUpdate(function(self, dt)
        local player_pos = char:getPosition()

        -- Check for death (always check, even if blinking)
        if player_pos.y <= -4 then
            print("PLAYER DIED! Respawning with blink effect...")
            -- Respawn player immediately
            phy:setPosition(x_val, y_val, z_val)
            score_component:setPoint(0)
            
            -- Start blinking effect
            is_blinking = true
            blink_elapsed = 0
            blink_timer = 0
            is_visible = true
        end

        -- Handle blinking effect after respawn
        if is_blinking then
            blink_elapsed = blink_elapsed + dt
            blink_timer = blink_timer + dt
            
            -- Toggle visibility at intervals
            if blink_timer >= blink_interval then
                blink_timer = 0
                is_visible = not is_visible
                
                -- Toggle visibility
                if skeletal_mesh.setIsVisible then
                    skeletal_mesh:setIsVisible(is_visible)
                elseif char.setIsVisible then
                    char:setIsVisible(is_visible)
                end
            end
            
            -- End blinking after duration
            if blink_elapsed >= blink_duration then
                is_blinking = false
                blink_elapsed = 0
                blink_timer = 0
                is_visible = true
                
                -- Make sure player is visible again
                if skeletal_mesh.setIsVisible then
                    skeletal_mesh:setIsVisible(true)
                elseif char.setIsVisible then
                    char:setIsVisible(true)
                end  
            end
        end
    end)
    
    char:start()
    return char
end
