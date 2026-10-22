return function(scene)
    print("GAME OBJECT: Creating redlight")
    local lightbulb = scene:addGameObject("redlight")
    local mesh_component
    local phy
    local particles
    local sfx
    
    -- Set collision callback AFTER component is created
    lightbulb:setLuaStart(function(self)
            print("GAME OBJECT: Creating lightbulb")
        mesh_component = lightbulb:addComponent("StaticMeshComponent", "Game/Assets/Models/sphere.obj", "Game/Assets/Textures/red.png")
        phy = lightbulb:addComponent("PhysicsComponent", "static", "trigger", 1, 1, 1)
        particles = lightbulb:addComponent("ParticleComponent", "Texture/circle_01.png", 100, false)
        sfx = lightbulb:addComponent("AudioComponent", "..\\assets\\audio\\SFX_box\\footstep_carpet_000.ogg", "collide1", true)
        
        lightbulb:setScale(1, 1, 1)
        lightbulb:setPosition(0, 0, 0)
        phy:setGravityEnabled(false)
        local initializedx;
        local initializedy;
        initializedx = math.random(-60, 60)
        initializedy = math.random(-60, 60)

        phy:setPosition(initializedx, 3, initializedy)
        light_component = lightbulb:addComponent("LightComponent", initializedx,3,initializedy ,1.0,0.0,0.0 ,5, 20,"Model/Fbx/sphere.fbx")


        particles:setEmissionRate(50)  -- 50 particles per second
        particles:setAutoEmit(false)    -- Automatically emit
        particles:setFollowOwner(true)

        -- Set callback inside start to ensure everything is initialized
        phy:setOnCollision(function(self, contactPoint, normal, depth, otherBody)

            phy:setPosition(math.random(-60, 60), 3, math.random(-60, 60))
            light_component:setPosition(phy:getPosition())
            for i = 1, 50 do
                particles:emit()
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
