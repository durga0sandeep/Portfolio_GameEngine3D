local MenuSystem = {}

function MenuSystem.initialize()
    print("Initializing Menu System")
    local mainMenu = MenuSystem.createMainMenu()
    local settingsMenu = MenuSystem.createSettings()
end

function MenuSystem.createMainMenu(scene)
    print("Creating Main Menu")
    
    local uiObject = scene:createGameObject("MainMenuUI")
    local uiComp = uiObject:addComponent("UIComponent", "MainMenu")
    
    local canvas = uiComp:getCanvas()
    
    canvas:addText(300, 500, "MAIN MENU", 2.0, 1.0, 1.0, 1.0)
    
    canvas:addTextButton(300, 350, "Start Game", function()
        MenuSystem.showGameUI()
    end)
    
    canvas:addTextButton(300, 280, "Options", function()
        MenuSystem.showSettings()
    end)
    
    canvas:addTextButton(300, 210, "Quit", function()
        MenuSystem.quit()
    end)
    
    return uiComp
end

function MenuSystem.createSettings(scene)
    print("Creating Settings Menu")
    
    local uiObject = scene:createGameObject("SettingsMenuUI")
    local uiComp = uiObject:addComponent("UIComponent", "SettingMenu")
    
    local canvas = uiComp:getCanvas()
    
    canvas:addText(250, 500, "Settings", 2.0, 1.0, 1.0, 1.0)
    
    canvas:addIconButton(350, 400, 50, 50,
        "Texture/btn_normal.png", "Texture/btn_focus.png", "Texture/btn_clicked.png",
        function() AudioManager.bgmVolumeUp() end
    )
    
    canvas:addTextButton(300, 150, "Back", function()
        MenuSystem.showMainMenu()
    end)
    
    return uiComp
end

function MenuSystem.createGameUI(scene)
    print("Creating Game UI")
    
    local uiObject = scene:createGameObject("GameUI")
    local uiComp = uiObject:addComponent("UIComponent", "GameUI")
    
    local canvas = uiComp:getCanvas()
    
    -- Add game UI elements here
    canvas:addText(50, 550, "Game UI", 1.5, 1.0, 1.0, 1.0)
    
    return uiComp
end

function MenuSystem.showMainMenu()
    -- UIManager.setActiveCanvas("MainMenu")
    if MenuSystem.mainMenuComp then
        MenuSystem.mainMenuComp:setActive(true)
    end
end

function MenuSystem.showSettings()
    -- UIManager.setActiveCanvas("SettingMenu")
end

function MenuSystem.showGameUI()
    -- UIManager.setActiveCanvas("GameUI")
end

function MenuSystem.quit()
    print("Quit button clicked!")
    -- TODOL: Add quit logic
end

return MenuSystem
