#pragma once

#include "Components/Component.h"
#include "UI/UIManager.h"

class UIButtonComponent : public Component
{
public:

    UIButtonComponent(glm::vec2 pos,
        const std::string& text,
        sol::function callback,
        bool defaultImg);
    UIButtonComponent(glm::vec2 pos,
        glm::vec2 size,
        const std::string& text,
        sol::function callback,
        bool defaultImg);

    ~UIButtonComponent() {};

    void setCustomTexture(const std::string& nomalTexPath,
        const std::string& focusedTexPath,
        const std::string& pressedTexPath);


    static void registerWithFactory(sol::state& lua);
    static void bindToLua(sol::state& lua);

    void setActive(bool input)
    {
        uiBtn->setActive(input);
    }

    void setOnClick()
    {
        uiBtn->onClick();
    }

    void setFullScreen(bool fullscreen);

private:
    UIButton* uiBtn = nullptr;
    glm::vec2 pos;
    glm::vec2 sz;
    std::string text;

    std::shared_ptr<Texture> normalTex = nullptr;
    std::shared_ptr<Texture> focusedTex = nullptr;
    std::shared_ptr<Texture> pressedTex = nullptr;
    bool useDefaultImg;

    bool isFullScreen = false;
};

