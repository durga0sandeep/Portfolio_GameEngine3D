#pragma once

#include "Components/Component.h"
#include "UI/UIManager.h"
#include "UI/UIText.h"

class UITextComponent : public Component
{
public:
    UITextComponent() {};
    UITextComponent(glm::vec2 _pos, const std::string& _content, float _scale ,glm::vec3 _color);
    ~UITextComponent() {};

    void addText(glm::vec2 pos, std::string text, float scale, glm::vec3 color)
    {
        uiText = &UIManager::instance().addText(pos, text, scale, color);
    }

    void setText(const std::string& newText)
    {
        uiText->setText(newText);
    }

    static void registerWithFactory(sol::state& lua);
    static void bindToLua(sol::state& lua);
    
private:
    UIText* uiText;
    glm::vec2 pos;
    const std::string& content = NULL;
    float textScale = NULL;
    glm::vec3 textColor ;
};

