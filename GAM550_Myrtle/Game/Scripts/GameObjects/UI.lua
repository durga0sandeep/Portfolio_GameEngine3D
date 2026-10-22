return function(scene)
    print("GAME OBJECT: Creating UI")
    local ui = scene:addGameObject("UI")
    local ui_component
    local ui_component1
    local ui_component2
    local ui_component3
    local ui_component4
    local ui_component5
    
    -- Set collision callback AFTER component is created
    ui:setLuaStart(function(self)
        print("GAME OBJECT: Creating UI")
        ui_component = ui:addComponent("UIComponent", "Texture/mute.png",20, 150, 80, 80,0)
        ui_component1 = ui:addComponent("UIComponent", "Texture/plus.png",120, 150,80, 80,1)
        ui_component2 = ui:addComponent("UIComponent", "Texture/fraction.png",220, 150,80, 100,2)
        ui_component3 = ui:addComponent("UIComponent", "Texture/effectMute.png",20, 300, 80, 80,3)
        ui_component4 = ui:addComponent("UIComponent", "Texture/plus.png",120, 300, 80, 80,4)
        ui_component5 =  ui:addComponent("UIComponent", "Texture/fraction.png",220, 300, 80, 80,5)
    end)
    
    ui:start()
    return ui
end
