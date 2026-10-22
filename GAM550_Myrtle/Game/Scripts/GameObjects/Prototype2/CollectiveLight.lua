return function(scene)
    --print("GAME OBJECT: Creating lightbulb")
    local lightbulb = scene:addGameObject("lightbulb")
    local mesh_component
    local phy
    local particles
    local sfx
    
    -- Set collision callback AFTER component is created
    lightbulb:setLuaStart(function(self)
            --print("GAME OBJECT: Creating lightbulb")
        mesh_component = lightbulb:addComponent("StaticMeshComponent", "Game/Assets/Models/lightBulb_base.obj", "Game/Assets/Textures/lightBulb_base.png")
        phy = lightbulb:addComponent("PhysicsComponent", "static", "trigger", 5, 10, 5)
        particles = lightbulb:addComponent("ParticleComponent", "Texture/circle_01.png", 100, false)
        sfx = lightbulb:addComponent("AudioComponent", "..\\assets\\audio\\SFX_box\\footstep_carpet_000.ogg", "collide", true)
        
        lightbulb:setScale(10, 10, 10)
        lightbulb:setPosition(0, 0, 0)
        phy:setGravityEnabled(false)
        phy:setPosition(0, 0, 0)

        particles:setEmissionRate(50)  -- 50 particles per second
        particles:setAutoEmit(false)    -- Automatically emit
        particles:setFollowOwner(true)

        -- Set callback inside start to ensure everything is initialized
        phy:setOnCollision(function(self, contactPoint, normal, depth, otherBody)
            --print("lightbulb COLLISION!")
            sfx:playSound()
            --print("Contact Y: " .. contactPoint.y)
            --print("Normal Y: " .. normal.y)

            phy:setPosition(math.random(-100, 100), 0, math.random(-100, 100))

            for i = 1, 50 do
                particles:emit()
            end
            if normal.y > 0.7 then
                --print("Hit ground!")
            end
        end)

    end)



    lightbulb:setLuaUpdate(function(self, dt)
        -- Get velocity from physics
        local vel = phy:getVelocity()
        local speed = math.sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z)
        
        -- Emit particles if moving fast enough
        if speed > 1.0 then
            --particles:emit()  -- Emit particles while moving
        end
    end)
    
    lightbulb:start()
    return lightbulb
end
