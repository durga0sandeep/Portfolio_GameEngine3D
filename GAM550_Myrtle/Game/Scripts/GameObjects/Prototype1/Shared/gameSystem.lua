return function(scene)
    print("GAME OBJECT: Creating box")
    local box = scene:addGameObject("box")
    local mesh_component
    local phy
    local particles
    local sfx
    
    -- Set collision callback AFTER component is created
    box:setLuaStart(function(self)
            print("GAME OBJECT: Creating box")
        mesh_component = box:addComponent("StaticMeshComponent", "Game/Assets/Models/centeredBox.obj", "Game/Assets/Textures/colormap.png")
        phy = box:addComponent("PhysicsComponent", "dynamic", "box", 5, 5, 5)
        particles = box:addComponent("ParticleComponent", "Texture/circle_01.png", 100, false)
        sfx = box:addComponent("AudioComponent", "..\\assets\\audio\\SFX_box\\footstep_carpet_000.ogg", "collide", true)

        box:setScale(10, 10, 10)
        phy:setGravityEnabled(true)
        phy:setPosition(0, 0, 0)

        particles:setEmissionRate(50)  -- 50 particles per second
        particles:setAutoEmit(false)    -- Automatically emit
        particles:setFollowOwner(true)

        -- Set callback inside start to ensure everything is initialized
        phy:setOnCollision(function(self, contactPoint, normal, depth, otherBody)
            print("BOX COLLISION!")
            sfx:playSound()
            print("Contact Y: " .. contactPoint.y)
            print("Normal Y: " .. normal.y)
            if normal.y > 0.7 then
                print("Hit ground!")
            end
        end)

    end)



    box:setLuaUpdate(function(self, dt)
        -- Get velocity from physics
        local vel = phy:getVelocity()
        local speed = math.sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z)
        
        -- Emit particles if moving fast enough
        if speed > 1.0 then
            particles:emit()  -- Emit particles while moving
        end
    end)
    
    box:start()
    return box
end
