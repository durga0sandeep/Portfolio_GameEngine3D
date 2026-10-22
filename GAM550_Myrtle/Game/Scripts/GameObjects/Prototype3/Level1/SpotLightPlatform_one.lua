return function(scene, x_val, y_val, z_val)
    print("GAME OBJECT: Creating spotlight_one")
    local lightbulb = scene:addGameObject("spotlight_one")
    local mesh_component
    local phy
    local particles
    local sfx
    local light_component
    local x_pos = x_val
    local y_pos = y_val
    local z_pos = z_val
    local target_x = x_val + 1000  -- Hidden position (far away)
    local appear = false  -- Start hidden
    local light_height = 10  -- Height above the platform
    
    -- Set collision callback AFTER component is created
    lightbulb:setLuaStart(function(self)
        print("GAME OBJECT: Creating lightbulb")
        mesh_component = lightbulb:addComponent("StaticMeshComponent", "Game/Assets/Models/centeredBox.obj", "Game/Assets/Textures/blue.png")
        phy = lightbulb:addComponent("PhysicsComponent", "static", "non_moving", 10, 2, 10)
        particles = lightbulb:addComponent("ParticleComponent", "Texture/smoke_01.png", 100, false)
        --sfx = lightbulb:addComponent("AudioComponent", "..\\assets\\audio\\SFX_box\\footstep_carpet_000.ogg", "collide2", true)
        lightbulb:setScale(20, 4, 20)
        phy:setGravityEnabled(false)
        
        -- Start in hidden position
        phy:setPosition(target_x, y_pos, z_pos)
        
        -- Light positioned directly above the platform
        light_component = lightbulb:addComponent("LightComponent", 
            target_x, y_pos + light_height, z_pos,  -- Same x,z as platform, higher y
            0.0, 0.0, 1.0,  -- Blue color
            0, 100)
        
        light_component:setScale(vec3(10, 50, 10))
        
        particles:setEmissionRate(50)  -- 50 particles per second
        particles:setAutoEmit(false)    -- Automatically emit
        particles:setFollowOwner(true)
        --phy:activate()
        
        lightbulb:setLuaTrigger(function(self)
            togglePos()
        end)
    end)
    
    function togglePos()
        print("Togggle from plat called")
        appear = not appear
        -- Calculate target position
        local new_x
        if appear then
            new_x = x_pos  -- Visible position
        else
            new_x = target_x  -- Hidden position
        end
        
        -- Move both platform and light together
        phy:setPosition(new_x, y_pos, z_pos)
        light_component:setPosition(vec3(new_x, y_pos + light_height, z_pos))
        
        print("Platform toggled: appear = " .. tostring(appear) .. ", x = " .. new_x)
        particles:emit()
    end
    
    lightbulb:start()
    return lightbulb
end
