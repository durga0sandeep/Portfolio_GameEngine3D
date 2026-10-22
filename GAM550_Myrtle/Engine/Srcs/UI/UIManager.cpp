#include "pch.h"
#include "UI/UIManager.h"
#include <Memory/AssetLoadManager.h>

/**
 * This is a declaration of the singleton for the Renderer Class
 *
 * \return
 */
UIManager& UIManager::instance()
{
    static UIManager instance;
    return instance;
}

/**
 * $BRIEF
 * initialize all resources(shader for the UI, mesh etc)
 */
UIManager::UIManager()
{
    panels.reserve(10);
    buttons.reserve(10);
    texts.reserve(10);
    UIPanel::initializePanelResources();
    UIButton::initializeButtonResources();
    UIButton::initializeBtnImage("Texture/btn_normal.png","Texture/btn_focus.png","Texture/btn_clicked.png");
    UIText::initializeTextResources("Font/arial.ttf");
}

UIPanel& UIManager::addPanel(glm::vec2 pos, glm::vec2 size, const std::string& texturePath)
{
    auto texture = AssetLoadManager::instance().assetTexture(texturePath.c_str());
    auto panel = std::make_shared<UIPanel>(pos, size, texture);
    panels.push_back(panel);
    return *panels.back();
}

/**
 * $BRIEF
 * add a button to the UI manager
 * \param pos
 * \param size
 * \param callback
 */
//UIButton& UIManager::addButton(glm::vec2 pos, glm::vec2 size, std::string texPath, std::function<void()> callback)
//{
//    auto button = std::make_shared<UIButton>(pos, size, texPath, callback);
//    buttons.push_back(button);
//    return *buttons.back();
//}

UIButton& UIManager::addButton(glm::vec2 pos, glm::vec2 size, const std::string text, sol::function callback)
{
    auto button = std::make_shared<UIButton>(pos, text, callback);
    buttons.push_back(button);
    return *buttons.back();
}

UIButton& UIManager::addButton(glm::vec2 pos, glm::vec2 size,
    std::shared_ptr<Texture> normalTex,
    std::shared_ptr<Texture> focusedTex,
    std::shared_ptr<Texture> pressedTex,
    sol::function callback)
{
    auto button = std::make_shared<UIButton>(pos, size, normalTex, focusedTex, pressedTex, callback);
    buttons.emplace_back(button);
    return *buttons.back();
}

UIText& UIManager::addText(glm::vec2 pos, std::string text, float scale, glm::vec3 color)
{
    texts.emplace_back(pos, text, scale, color);
    return texts.back();
}

/**
 * $BRIEF
 * handle mouse click event
 * \param mouseX
 * \param mouseY
 */
void UIManager::handleClick(float mouseX, float mouseY)
{
    for (auto& button : buttons) {
        if (button->isActive())
        if (button->isPointInside(mouseX, mouseY)) {
            //std::cout << "btnClick" << std::endl;
            if (button->onClick) {
                button->onClick();
            }
            break;
        }
    }
}




//void UIManager::handleInput(const std::string& input) {
//    //if (!activeCanvas) return;
//
//    if (input == "UP" || input == "LEFT") {
//        activeCanvas->focusPrevious();
//    }
//    else if (input == "DOWN" || input == "RIGHT") {
//        activeCanvas->focusNext();
//    }
//    else if (input == "ACTIVATE" || input == "ENTER") {
//        activeCanvas->activateFocused();
//    }
//}
//
//void UIManager::handleMouseClick(float mouseX, float mouseY) {
//  /*  if (activeCanvas) {
//        activeCanvas->handleMouseClick(mouseX, mouseY);
//    }*/
//}

/**
 * $BRIEF
 * render all UI elements
 */
void UIManager::render()
{
    for (auto& panel : panels)
    {
        if(panel->isActive())
        panel->draw();
    }
    for(auto button : buttons)
    {
        if (button->isActive())
        button->draw();
    }
    for (auto text : texts)
    {
        if (text.isActive())
        text.draw();
    }

}
