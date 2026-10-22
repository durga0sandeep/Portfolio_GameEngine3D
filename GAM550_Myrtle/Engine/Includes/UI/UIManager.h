/*!****************************************************************************
 * file  : UIManager.h
 * brief : class to store and handle all UI elements such as buttons
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include "UIButton.h"
#include <vector>
#include <functional>
#include <iostream>
#include "UIText.h"
#include "UICanvas.h"
#include "UI/UIPanel.h"

class UIManager {
public:
    // Access the singleton instance
    static UIManager& instance();

    // Delete copy/move constructors to enforce singleton
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;
    UIManager(UIManager&&) = delete;
    UIManager& operator=(UIManager&&) = delete;

    UIPanel& addPanel(glm::vec2 pos, glm::vec2 size, const std::string& texturePath);

    //UIButton& addButton(glm::vec2 pos, glm::vec2 size, std::string texPath, std::function<void()> callback);
    UIButton& addButton(glm::vec2 pos, glm::vec2 size, const std::string text, sol::function callback); // for default img
    UIButton& addButton(glm::vec2 pos, glm::vec2 size,
        std::shared_ptr<Texture> normalTex,
        std::shared_ptr<Texture> focusedTex,
        std::shared_ptr<Texture> pressedTex,
        sol::function callback);

    UIText& addText(glm::vec2 pos, std::string text,float scale, glm::vec3 color);

    void focusNext();
    void focusPrevious();
    void activateFocused();
    void updateFocus();


    void handleClick(float mouseX, float mouseY);
   
    /*std::shared_ptr<UICanvas> createCanvas(const std::string& name);
    std::shared_ptr<UICanvas> getCanvas(const std::string& name);
    void setActiveCanvas(const std::string& name);
    std::shared_ptr<UICanvas> getActiveCanvas();*/

    // Input handling for active canvas
    void handleInput(const std::string& input); // "UP", "DOWN", "ACTIVATE"
    void handleMouseClick(float mouseX, float mouseY);

    void render();

    void clear()
    {
        panels.clear();
        buttons.clear();
        texts.clear();
        focusedIndex = -1;
    }

private:
    // Private constructor
    UIManager();

    std::vector<std::shared_ptr<UIPanel>> panels;
    std::vector<std::shared_ptr<UIButton>> buttons;
    std::vector<UIText> texts;

    int findNextEnabledButton(int startIndex, bool forward);
    int focusedIndex = -1;

    //std::unordered_map<std::string, std::shared_ptr<UICanvas>> canvases;
    //std::shared_ptr<UICanvas> activeCanvas;
};


