    return function(scene)
        print("GAME OBJECT: Creating sky")
        local sky = scene:addGameObject("Sky")
        local sphere_component 

        -- Set the Lua start function
        sky:setLuaStart(function(self)
            sphere_component = sky:addComponent("SkySphereComponent", "Model/Obj/sky_icoshpere.obj", "Texture/qwantani_sunset_puresky.jpg")
            print("SKYYYYY STARTT!!!")
        end)
    
       -- Set the Lua update function
       --sky:setLuaUpdate(function(self, dt)
       --    print("Sky is updating!")
       --end)
        sky:start()

        return sky
    end
