/*!****************************************************************************
 * file  : UICanvas.h
 * brief : This class is Container for a UI screen/menu
 * Each canvas represents one menu (main menu, pause menu, options, etc.)
 *
 * author: YenLing Chang
 * Course: CS550
 * date  : 11/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/


#pragma once
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include <Graphics/Texture.h>
#include "UI/UIPanel.h"
#include <sol/sol.hpp>

class UICanvas
{
public:
    UICanvas(const std::string& name);
    ~UICanvas();

    void activate();
    void deactivate();
    bool isActive() const { return m_isActive; }

    const std::string& getName() const { return canvasName; }

    void addPanel(std::shared_ptr<UIPanel> panel);

    void addButton(glm::vec2 pos, const std::string& text, sol::function callback);
    void addIconButton(glm::vec2 pos, glm::vec2 size,
        std::shared_ptr<Texture> normalTex,
        std::shared_ptr<Texture> focusedTex,
        std::shared_ptr<Texture> pressedTex,
        sol::function callback);

    void addText(std::shared_ptr<class UIText> text);

    void clear();

    // Focus navigation (called by input system)
    void focusNext();
    void focusPrevious();
    void activateFocused();

    void handleMouseClick(float x, float y);

    void render();

    int getFocusedIndex() const { return focusedIndex; }


private:
    std::string canvasName;
    std::vector<std::shared_ptr<UIPanel>> panels;
    std::vector<std::shared_ptr<class UIButton>> buttons;
    std::vector<std::shared_ptr<class UIText>> texts;

    int focusedIndex; // -1 if none
    bool m_isActive = false;

    void updateFocus();
    int findNextEnabledButton(int startIndex, bool forward);
};

