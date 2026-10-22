return function(scene, x_val, y_val, z_val)
    print("GAME OBJECT: Creating sequential platform system")
    local platform_system = scene:addGameObject("sequential_platforms")
    
    -- Platform 1
    local platform1 = scene:addGameObject("platform_1")
    local mesh1, phy1, light1
    
    -- Platform 2
    local platform2 = scene:addGameObject("platform_2")
    local mesh2, phy2, light2
    
    -- Platform 3
    local platform3 = scene:addGameObject("platform_3")
    local mesh3, phy3, light3
    
    local x_pos = x_val
    local y_pos = y_val
    local z_pos = z_val
    local platform_spacing = 50
    local hidden_offset = 1000
    local light_height = 10
    
    -- Timing variables
    local is_active = false
    local sequence_timer = 0
    local platform1_activated = false
    local platform2_activated = false
    local platform3_activated = false
    local platform1_on_time = 0
    local platform2_on_time = 0
    local platform3_on_time = 0
    
    platform_system:setLuaStart(function(self)
        print("Setting up sequential platforms")
        
        -- PLATFORM 1 SETUP
        mesh1 = platform1:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/centeredBox.obj", 
            "Game/Assets/Textures/red.png")
        phy1 = platform1:addComponent("PhysicsComponent", "static", "non_moving", 10, 2, 10)
        platform1:setScale(20, 4, 20)
        phy1:setGravityEnabled(false)
        phy1:setPosition(x_pos + hidden_offset, y_pos, z_pos)  -- Start hidden
        
        light1 = platform1:addComponent("LightComponent",
            x_pos + hidden_offset, y_pos + light_height, z_pos,
            1.0, 0.0, 0.0,  -- Red
            0, 100)
        light1:setScale(vec3(10, 50, 10))
        
        -- PLATFORM 2 SETUP
        mesh2 = platform2:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/centeredBox.obj", 
            "Game/Assets/Textures/green.png")
        phy2 = platform2:addComponent("PhysicsComponent", "static", "non_moving", 10, 2, 10)
        platform2:setScale(20, 4, 20)
        phy2:setGravityEnabled(false)
        phy2:setPosition(x_pos + hidden_offset, y_pos, z_pos + platform_spacing)  -- Start hidden
        
        light2 = platform2:addComponent("LightComponent",
            x_pos + hidden_offset, y_pos + light_height, z_pos + platform_spacing,
            0.0, 1.0, 0.0,  -- Green
            0, 100)
        light2:setScale(vec3(10, 50, 10))
        
        -- PLATFORM 3 SETUP
        mesh3 = platform3:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/centeredBox.obj", 
            "Game/Assets/Textures/blue.png")
        phy3 = platform3:addComponent("PhysicsComponent", "static", "non_moving", 10, 2, 10)
        platform3:setScale(20, 4, 20)
        phy3:setGravityEnabled(false)
        phy3:setPosition(x_pos + hidden_offset, y_pos, z_pos + platform_spacing * 2)  -- Start hidden
        
        light3 = platform3:addComponent("LightComponent",
            x_pos + hidden_offset, y_pos + light_height, z_pos + platform_spacing * 2,
            0.0, 0.0, 1.0,  -- Blue
            0, 100)
        light3:setScale(vec3(10, 50, 10))
        
        platform1:start()
        platform2:start()
        platform3:start()
        
        -- Set up trigger to start the sequence
        platform_system:setLuaTrigger(function(self)
            if not is_active then
                print("Sequential platforms triggered!")
                is_active = true
                sequence_timer = 0
                platform1_activated = false
                platform2_activated = false
                platform3_activated = false
            end
        end)
    end)
    
    platform_system:setLuaUpdate(function(self, dt)
        if is_active then
            sequence_timer = sequence_timer + dt
            
            -- Turn ON platform 1 at t=0
            if not platform1_activated and sequence_timer >= 0.0 then
                phy1:setPosition(x_pos, y_pos, z_pos)
                light1:setPosition(vec3(x_pos, y_pos + light_height, z_pos))
                platform1_activated = true
                platform1_on_time = sequence_timer
                print("Platform 1 ON at t=" .. sequence_timer)
            end
            
            -- Turn ON platform 2 at t=1
            if not platform2_activated and sequence_timer >= 1.0 then
                phy2:setPosition(x_pos, y_pos, z_pos + platform_spacing)
                light2:setPosition(vec3(x_pos, y_pos + light_height, z_pos + platform_spacing))
                platform2_activated = true
                platform2_on_time = sequence_timer
                print("Platform 2 ON at t=" .. sequence_timer)
            end
            
            -- Turn ON platform 3 at t=2
            if not platform3_activated and sequence_timer >= 3.0 then
                phy3:setPosition(x_pos, y_pos, z_pos + platform_spacing * 2)
                light3:setPosition(vec3(x_pos, y_pos + light_height, z_pos + platform_spacing * 2))
                platform3_activated = true
                platform3_on_time = sequence_timer
                print("Platform 3 ON at t=" .. sequence_timer)
            end
            
            -- Turn OFF platform 1 after 2 seconds (at t=2)
            if platform1_activated and sequence_timer >= platform1_on_time + 3.0 and phy1:getPosition().x < hidden_offset then
                phy1:setPosition(x_pos + hidden_offset, y_pos, z_pos)
                light1:setPosition(vec3(x_pos + hidden_offset, y_pos + light_height, z_pos))
                print("Platform 1 OFF at t=" .. sequence_timer)
            end
            
            -- Turn OFF platform 2 after 2 seconds (at t=3)
            if platform2_activated and sequence_timer >= platform2_on_time + 3.0 and phy2:getPosition().x < hidden_offset then
                phy2:setPosition(x_pos + hidden_offset, y_pos, z_pos + platform_spacing)
                light2:setPosition(vec3(x_pos + hidden_offset, y_pos + light_height, z_pos + platform_spacing))
                print("Platform 2 OFF at t=" .. sequence_timer)
            end
            
            -- Turn OFF platform 3 after 2 seconds (at t=4) and end sequence
            if platform3_activated and sequence_timer >= platform3_on_time + 3.0 then
                phy3:setPosition(x_pos + hidden_offset, y_pos, z_pos + platform_spacing * 2)
                light3:setPosition(vec3(x_pos + hidden_offset, y_pos + light_height, z_pos + platform_spacing * 2))
                print("Platform 3 OFF at t=" .. sequence_timer)
                print("Sequence complete!")
                is_active = false
            end
        end
    end)
    
    platform_system:start()
    return platform_system
end
