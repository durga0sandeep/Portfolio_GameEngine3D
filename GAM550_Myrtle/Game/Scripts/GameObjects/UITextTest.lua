    return function(scene)
        print("GAME OBJECT: Creating UI TEST")
        local UI = scene:addGameObject("UITest")
        local text 
        local btn

        -- Set the Lua start function
        UI:setLuaStart(function(self)
            text = UI:addComponent("UITextComponent", 50,80,"TESTING TEXT!", 1, vec3(1,1,1))
            --btn = UI:addComponent("UIButtonComponent", 50,80,"PRESS",
            --function()           
            --    print("Button was pressed!")
--                text:setText("Button Clicked!")
--            end ,
--            true)
            print("UI TEST CREATED!!!!!!!!")
        end)
    
       -- Set the Lua update function
       --sky:setLuaUpdate(function(self, dt)
       --    print("Sky is updating!")
       --end)
        UI:start()

        return UI
    end
