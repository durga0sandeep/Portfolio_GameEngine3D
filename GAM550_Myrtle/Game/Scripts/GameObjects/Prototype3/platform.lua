return function(scene, x_val, y_val, z_val)
    print("GAME OBJECT: Creating box")
    local box = scene:addGameObject("platform")
    local mesh_component
    local phy
    local x_pos = x_val
    local y_pos = y_val
    local z_pos = z_val
    
    box:setLuaStart(function(self)
        print("GAME OBJECT: Creating hanging box")
        mesh_component = box:addComponent("StaticMeshComponent", "Game/Assets/Models/centeredBox.obj", "Game/Assets/Textures/colormap.png")
        phy = box:addComponent("PhysicsComponent", "static", "non_moving", 40, 2, 15)
        
        phy:setPosition(x_pos, y_pos, z_pos)
        box:setScale(80, 4, 30)
        phy:setGravityEnabled(true)
        
        phy:setOnCollision(function(self, contactPoint, normal, depth, otherBody)
            print("BOX COLLISION!")
            --sfx:playSound()
            --print("Contact Y: " .. contactPoint.y)
            --print("Normal Y: " .. normal.y)
            --if normal.y > 0.7 then
            --    print("Hit ground!")
            --end
        end)
        
        box:setLuaTrigger(function(self)
            --rope:detach()
        end)
    end)
    
    box:setLuaUpdate(function(self, dt)
        -- Get velocity from physics
        --local vel = phy:getVelocity()
        --local speed = math.sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z)
        
        -- Emit particles if moving fast enough
        --if speed > 1.0 then
        --    particles:emit()  -- Emit particles while moving
        --end
    end)
    
    box:start()
    return box
end
