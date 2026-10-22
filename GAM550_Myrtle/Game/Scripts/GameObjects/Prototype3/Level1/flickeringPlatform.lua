return function(scene, x_val, y_val, z_val)
    local flickeringPlat = scene:addGameObject("flickering_light")
    local mesh_component
    local phy
    local light_component
    local x_pos = x_val
    local y_pos = y_val
    local z_pos = z_val
    local target_x = x_val + 1000
    local appear = true
    local light_height = 10
    local flicker_timer = 0
    local flicker_interval = 2.0
    
    flickeringPlat:setLuaStart(function(self)
        print("GAME OBJECT: Creating flickeringPlat")
        mesh_component = flickeringPlat:addComponent("StaticMeshComponent", "Game/Assets/Models/centeredBox.obj", "Game/Assets/Textures/green.png")
        phy = flickeringPlat:addComponent("PhysicsComponent", "static", "non_moving", 10, 2, 10)
        flickeringPlat:setScale(20, 4, 20)
        phy:setGravityEnabled(false)
        phy:setPosition(x_pos, y_pos, z_pos)
        
        light_component = flickeringPlat:addComponent("LightComponent",
            x_pos, y_pos + light_height, z_pos,
            0.0, 1.0, 0.0,
            0, 100)
        light_component:setScale(vec3(10, 50, 10))
        -- NO setLuaTrigger here!
    end)
    
    function toggleFlickering()
        appear = not appear
        local new_x
        if appear then
            new_x = x_pos
        else
            new_x = target_x
        end
        phy:setPosition(new_x, y_pos, z_pos)
        light_component:setPosition(vec3(new_x, y_pos + light_height, z_pos))
    end
    
    flickeringPlat:setLuaUpdate(function(self, dt)
        flicker_timer = flicker_timer + dt
        if flicker_timer >= flicker_interval then
            flicker_timer = 0
            toggleFlickering()
        end
    end)
    
    flickeringPlat:start()
    return flickeringPlat
end
