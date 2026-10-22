    return function(scene)
        --print("GAME OBJECT: Creating sky")
        local bgm = scene:addGameObject("bgm")
        local AudioComponent 

        -- Set the Lua start function
        bgm:setLuaStart(function(self)
            AudioComponent = bgm:addComponent("AudioComponent", "..\\assets\\audio\\temp_bgm.wav", "bgm", false)
            AudioComponent:playBGMusic()
        end)
    
       -- Set the Lua update function
       --sky:setLuaUpdate(function(self, dt)
       --    print("Sky is updating!")
       --end)
        bgm:start()

        return bgm
    end
