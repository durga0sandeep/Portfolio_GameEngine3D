#include "pch.h"
#include "UI/UICanvas.h"
#include "UI/UIButton.h"
#include "UI/UIText.h"

UICanvas::UICanvas(const std::string& name)
    : canvasName(name), focusedIndex(-1), m_isActive(false) {
}

UICanvas::~UICanvas() {
    clear();
}


void UICanvas::activate() {
    m_isActive = true;

    // Focus first enabled button
    if (!buttons.empty()) {
        focusedIndex = findNextEnabledButton(-1, true);
        updateFocus();
    }
}

void UICanvas::deactivate() {
    // Clear focus from all buttons
    for (auto& button : buttons) {
        button->setFocused(false);
    }

    focusedIndex = -1;
    m_isActive = false;
}

void UICanvas::addPanel(std::shared_ptr<UIPanel> panel) {
    if (panel) {
        panels.push_back(panel);
    }
}

void UICanvas::addButton(glm::vec2 pos, const std::string& text, sol::function callback) {
    auto button = std::make_shared<UIButton>(pos, text, callback);
    buttons.push_back(button);
}

void UICanvas::addIconButton(glm::vec2 pos, glm::vec2 size,
    std::shared_ptr<Texture> normalTex,
    std::shared_ptr<Texture> focusedTex,
    std::shared_ptr<Texture> pressedTex,
    sol::function callback) {
    auto button = std::make_shared<UIButton>(pos, size, normalTex, focusedTex, pressedTex, callback);
    buttons.push_back(button);
}

void UICanvas::addText(std::shared_ptr<UIText> text) {
    if (text) {
        texts.push_back(text);
    }
}

void UICanvas::clear() {
    buttons.clear();
    texts.clear();
    focusedIndex = -1;
}

int UICanvas::findNextEnabledButton(int startIndex, bool forward) {
    if (buttons.empty()) return -1;

    int count = static_cast<int>(buttons.size());
    int index = startIndex;

    // Search for next enabled button
    for (int i = 0; i < count; ++i) {
        if (forward) {
            index = (index + 1) % count;
        }
        else {
            index = (index - 1 + count) % count;
        }

        if (buttons[index]->isEnabled()) {
            return index;
        }
    }

    return startIndex; // All disabled, stay on current
}

void UICanvas::updateFocus() {
    // Remove focus from all buttons
    for (auto& button : buttons) {
        button->setFocused(false);
    }

    // Set focus on current
    if (focusedIndex >= 0 && focusedIndex < static_cast<int>(buttons.size())) {
        buttons[focusedIndex]->setFocused(true);
    }
}

void UICanvas::focusNext() {
    if (!m_isActive || buttons.empty()) return;

    focusedIndex = findNextEnabledButton(focusedIndex, true);
    updateFocus();
}

void UICanvas::focusPrevious() {
    if (!m_isActive || buttons.empty()) return;

    focusedIndex = findNextEnabledButton(focusedIndex, false);
    updateFocus();
}

void UICanvas::activateFocused() {
    if (!m_isActive) return;

    if (focusedIndex >= 0 && focusedIndex < static_cast<int>(buttons.size())) {
        buttons[focusedIndex]->activate();
    }
}

void UICanvas::handleMouseClick(float x, float y) {
    for (size_t i = 0; i < buttons.size(); ++i) {
        auto& button = buttons[i];
        if (button->isPointInside(x, y) && button->isEnabled()) {
            // Update focus to clicked button
            focusedIndex = static_cast<int>(i);
            updateFocus();

            // Activate it
            button->activate();
            break;
        }
    }
}

void UICanvas::render() {
    for (auto& panel : panels) {
        panel->draw();
    }

    for (auto& button : buttons) {
        button->draw();
    }

    for (auto& text : texts) {
        text->draw();
    }
}
