return function(scene, x_val, y_val, z_val)
    print("GAME OBJECT: Creating skeleton")
    local char = scene:addGameObject("Player")
    local skeletal_mesh
    local phy
    local player_controller
    local trigger

    local within_range = false
    local other_gameobj = nil
    local interaction_distance = 10.0

    print ("player controller added")

    --Set the Lua start function
    char:setLuaStart(function(self)
        char:setScale(0.1, 0.1, 0.1)
        skeletal_mesh = char:addComponent("SkeletalMeshComponent", "Game/Assets/AnimClips/Player/Idle.fbx")
        phy = char:addComponent("PhysicsComponent", "dynamic", "player", 2, 3.75, 2,true)
        player_controller = char:addComponent("PlayerController")
        trigger = char:addComponent("TriggerComponent")
        phy:setPosition(x_val, y_val, z_val)
        print("THE PLAYER IS SETUP")

        -- Configure movement
        print("starting to configure controller")
        player_controller:setMoveSpeed(20.0)
        player_controller:setRunSpeedMultiplier(2.0)
        player_controller:setJumpForce(1600000.0 * 4)
        print("movement set!")
        
        -- Configure camera
        player_controller:setCameraDistance(40.0)
        player_controller:setCameraSensitivity(0.15)
        print ("camera set!")
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
            if otherGameObject:getName() == "lever" then
                print("PLAYER TOUCHED LEVER!")
                within_range = true
                other_gameobj = otherGameObject
            end
        end)
    end)

    char:setLuaUpdate(function(self, dt)
        if other_gameobj ~= nil then
            --print("other body is not null")
            -- Get positions (you'll need to adapt this to your engine's API)
            local player_pos = char:getPosition()
            local other_pos = other_gameobj:getPosition()
            
            -- Calculate distance
            local dx = player_pos.x - other_pos.x
            local dy = player_pos.y - other_pos.y
            local dz = player_pos.z - other_pos.z
            local distance = math.sqrt(dx*dx + dy*dy + dz*dz)

            -- Update the boolean based on distance
            if distance > interaction_distance then
                if within_range then
                    print("Player moved away from lever")
                    within_range = false
                    other_gameobj = nil
                end
            else
                if player_controller:getActionPressed() then
                    print("Action button pressed within range")
                    trigger:setOther(other_gameobj)
                    trigger:action()
                end
                --print("close to lever")
            end
        end
    end)


   char:start()

    return char
end
