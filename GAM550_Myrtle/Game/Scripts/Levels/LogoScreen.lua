return function(scene)
    print("Creating Logo Screen")
    
    local uiLogoScreen = scene:addGameObject("uiLogoScreen")
    local btn
    local currentBtn = nil
    local logoTimer = 0
    local logoDuration = 2.0  
    local currentLogoIndex = 1

    local logoImages = {
        "Texture/logo.png",
        "Texture/logo_f.png"
    }

    local function createLogoButton(imagePath)
        local newBtn = uiLogoScreen:addComponent("UIButtonComponent", 
            0, 0, 
            "",   
            function()
                print("Logo clicked, skipping to main menu...")
                switchToMainMenu()
            end,
            false 
        )
        
        if newBtn then
            newBtn:setCustomTexture(imagePath, imagePath, imagePath)
        end
        
        return newBtn
    end
    
    local function nextLogo()
        currentLogoIndex = currentLogoIndex + 1
        if currentLogoIndex > 3 then
            print("All logos shown, switching to main menu...")
            switchToMainMenu()
            return
        end
        
        currentBtn = createLogoButton(logoImages[currentLogoIndex])
        
        logoTimer = 0
    end

    local function switchToMainMenu()
        scene:setLevelCompleted(true)
        
        print("Main menu loaded")
    end



    uiLogoScreen:setLuaUpdate(function(self, dt)
        print(logoTimer)
        logoTimer = logoTimer + dt
        if logoTimer >= logoDuration then
            nextLogo()
        end
    end)



    uiLogoScreen:setLuaStart(function(self)
        currentBtn = createLogoButton(logoImages[1])
        currentBtn:setCustomTexture(
            logoImages[1],
            logoImages[1],
            logoImages[1]
        )
--       btn = uiLogoScreen:addComponent("UIButtonComponent", 0,0,"",
--       function()           
--           btn->setCustomTexture("Texture/logo.png",
--               "Texture/logo.png",  
--               "Texture/logo.png")
--       end ,
--       false)
--       panel = uiLogoScreen:addComponent("UIPanelComponent", 50,80,300,300,"Texture/ui_bg.png")
    end)
    
    
    uiLogoScreen:start()
    
    print("Logo Screen Created")
    return uiLogoScreen
    
end
