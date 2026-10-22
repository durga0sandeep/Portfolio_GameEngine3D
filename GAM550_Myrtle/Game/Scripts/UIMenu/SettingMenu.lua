return function(scene)
    print("=== Creating setting Menu ===")
    local ui_setting = scene:addGameObject("SettingUI")
    local bg_panel
    local mute_bgm
    local mute_sfx
    local volumeup_bgm
    local volumeup_sfx
    local volumedown_bgm
    local volumedown_sfx
    local back_btn
    local inputComponent
    local audioComponent
    
    -- Menu state variables
    local menu_active = false
    local selected_index = 1
    local buttons = {}  -- Will store all buttons in order
    local m_pressed = false
    local up_pressed = false
    local down_pressed = false
    local enter_pressed = false
                 
    ui_setting:setLuaStart(function(self)
        print("Setting up Setting UI components...")
        
        bg_panel = ui_setting:addComponent("UIPanelComponent", 
            0,                        
            0,                       
            800,                       
            600,       
            "Texture/ui_bg.png"    
        )
        audioComponent = ui_setting:addComponent("AudioComponent", "..\\assets\\audio\\temp_bgm.wav", "bgm", false)
        mute_bgm = ui_setting:addComponent("UIButtonComponent", 
            150,150,"",
            function()           
                print("mute_bgm Button was pressed!")
                audioComponent:stopBGM()
            end ,
            false           
        )
        mute_bgm:setCustomTexture("Texture/mute.png", "Texture/mute.png", "Texture/mute.png")
       
        mute_sfx = ui_setting:addComponent("UIButtonComponent", 
            150,250,"",
            function()           
                print("mute_sfx Button was pressed!")
                audioComponent:stopSFX()
            end ,
            false           
        )
        mute_sfx:setCustomTexture("Texture/effectMute.png", "Texture/effectMute.png", "Texture/effectMute.png")
        
        volumeup_bgm = ui_setting:addComponent("UIButtonComponent", 
            350,150,"+",
            function()           
                print("volumeup_bgm Button was pressed!")
                audioComponent:bgmVolumeUp()
            end ,
            true           
        )
        volumeup_sfx = ui_setting:addComponent("UIButtonComponent", 
            350,280,"+",
            function()           
                print("volumeup_sfx Button was pressed!")
                audioComponent:sfxVolumeUp()
            end ,
            true           
        )
        volumedown_bgm = ui_setting:addComponent("UIButtonComponent", 
            750,180,"-",
            function()           
                print("volumedown_bgm Button was pressed!")
                audioComponent:bmgVolumeDown()
            end ,
            true           
        )
        volumedown_sfx = ui_setting:addComponent("UIButtonComponent", 
            750,280,"-",
            function()           
                print("volumedown_sfx Button was pressed!")
            end ,
            true           
        )
        
        back_btn = ui_setting:addComponent("UIButtonComponent", 
            250,380,"Back",
            function()           
                print("quit Button was pressed!")
                scene:setChangeLevel("menu")
            end ,
            true           
        )
        
        inputComponent = ui_setting:addComponent("InputComponenet",0,0)
        
        --Store buttons in navigation order
        buttons = {
            mute_bgm,
            mute_sfx,
            volumeup_bgm,
            volumeup_sfx,
            volumedown_bgm,
            volumedown_sfx,
            back_btn
        }
        
        --Hide menu initially
        bg_panel:setActive(false)
        for i, btn in ipairs(buttons) do
            btn:setActive(false)
        end
        
        print("Setting menu initialized. Press M to toggle.")
    end)
    
    ui_setting:setLuaUpdate(function(self, dt)
        -- Check for M key press (toggle menu)
        local m_key = inputComponent:isButtonClick("m")  -- 77 is 'M' key code
        if m_key and not m_pressed then
            m_pressed = true
            menu_active = not menu_active
            --print("Menu toggled:", menu_active)
            
            -- Show/hide all UI elements
            bg_panel:setActive(menu_active)
            for i, btn in ipairs(buttons) do
                btn:setActive(menu_active)
            end
            
            -- Reset selection when opening menu
            if menu_active then
                selected_index = 1
                buttons[selected_index]:setFocused(true)
            else
                -- Clear focus when closing
                for i, btn in ipairs(buttons) do
                    btn:setFocused(false)
                end
            end
        elseif not m_key then
            m_pressed = false
        end
        
        -- -- Only process navigation if menu is active
        -- if not menu_active then
        --     return
        -- end
        
        -- -- Check for Up key press
        -- local up_key = inputComponent:isButtonClick("up")  -- Up arrow key code
        -- if up_key and not up_pressed then
        --     up_pressed = true
            
        --     -- Clear current focus
        --     buttons[selected_index]:setFocused(false)
            
        --     -- Move selection up
        --     selected_index = selected_index - 1
        --     if selected_index < 1 then
        --         selected_index = #buttons  -- Wrap to bottom
        --     end
            
        --     -- Set new focus
        --     buttons[selected_index]:setFocused(true)
        --     print("Selected button index:", selected_index)
        -- elseif not up_key then
        --     up_pressed = false
        -- end
        
        -- -- Check for Down key press
        -- local down_key = inputComponent:isButtonClick("down")  -- Down arrow key code
        -- if down_key and not down_pressed then
        --     down_pressed = true
            
        --     -- Clear current focus
        --     buttons[selected_index]:setFocused(false)
            
        --     -- Move selection down
        --     selected_index = selected_index + 1
        --     if selected_index > #buttons then
        --         selected_index = 1  -- Wrap to top
        --     end
            
        --     -- Set new focus
        --     buttons[selected_index]:setFocused(true)
        --     print("Selected button index:", selected_index)
        -- elseif not down_key then
        --     down_pressed = false
        -- end
        
        -- -- Check for Enter key press (activate selected button)
        -- local enter_key = inputComponent:isButtonClick("enter")  -- Enter key code
        -- if enter_key and not enter_pressed then
        --     enter_pressed = true
            
        --     -- Trigger the onClick callback of the selected button
        --     print("Activating button:", selected_index)
        --     buttons[selected_index]:triggerOnClick()
        -- elseif not enter_key then
        --     enter_pressed = false
        -- end
    end)
    
    ui_setting:start()
    return ui_setting
end
