return function(scene)
    print("Creating Logo Screen")
    
    local uiLogoScreen = scene:addGameObject("uiLogoScreen")
    local btn
    local currentBtn = nil
    local logoTimer = 0
    local logoDuration = 1.0  
    local currentLogoIndex = 1

    local done = false

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
            newBtn:setFullScreen(true) 
        end
        
        return newBtn
    end

    local function switchToMainMenu()
        scene:setLevelCompleted(true)
        done = true
        print("Main menu loaded")
    end

    local function nextLogo()
        currentLogoIndex = currentLogoIndex + 1
        if currentLogoIndex > 2 then
            print("All logos shown, switching to main menu...")
            switchToMainMenu()
            return
        end
        
        currentBtn = createLogoButton(logoImages[currentLogoIndex])
        
        logoTimer = 0
    end

    uiLogoScreen:setLuaUpdate(function(self, dt)
        print(logoTimer)
        logoTimer = logoTimer + dt
        if logoTimer >= logoDuration and not done then
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
    end)
    
    
    uiLogoScreen:start()
    
    print("Logo Screen Created")
    return uiLogoScreen
    
end
