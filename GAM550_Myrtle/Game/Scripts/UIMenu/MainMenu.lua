return function(scene)
    print("GAME OBJECT: Creating MainMenuUI")
    
    local ui = scene:addGameObject("MainMenuUI")
    local bg_panel
    local start_btn
    local setting_btn
    local quit_btn
    
    ui:setLuaStart(function(self)
        print("Setting up UI components...")
        
        bg_panel = ui:addComponent("UIPanelComponent", 
            0,                        
            0,                       
            800,                       
            600,       
            "Texture/ui_bg.png"    
        )
        
        start_btn = ui:addComponent("UIButtonComponent", 
            250,180,"Start1",
            function()           
                print("start1 Button was pressed!")
                scene:setChangeLevel("prototype1")
            end ,
            true           
        )

        start_btn = ui:addComponent("UIButtonComponent", 
            250,280,"Start2",
            function()           
                 scene:setChangeLevel("prototype2")
            end ,
            true           
        )

        start_btn = ui:addComponent("UIButtonComponent", 
            250,380,"Start3",
            function()           
                 scene:setChangeLevel("prototype3")
            end ,
            true           
        )
       
        setting_btn = ui:addComponent("UIButtonComponent", 
            250,480,"Setting",
            function()           
                print("setting Button was pressed!")
            end ,
            true           
        )
        
        quit_btn = ui:addComponent("UIButtonComponent", 
            250,580,"Quit",
            function()           
                print("quit Button was pressed!")
            end ,
            true           
        )
        
        
    end)
    
    ui:start()
    return ui
end
