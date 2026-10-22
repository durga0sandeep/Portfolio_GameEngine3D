#include "pch.h"
#include "Components/UIButtonComponent.h"
#include "Components/ComponentFactory.h"
#include "Entities/GameObject.h"
#include "Engine.h"


UIButtonComponent::UIButtonComponent(glm::vec2 _pos, const std::string& _text, sol::function _callback, bool defaultImg)
    : pos(_pos), text(_text), sz(glm::vec2(200, 100)), useDefaultImg(true)
{
    if (defaultImg) {
        uiBtn->initializeBtnImage("Texture/btn_normal.png", "Texture/btn_focus.png", "Texture/btn_clicked.png");
        uiBtn = &UIManager::instance().addButton(_pos, glm::vec2(200, 100), _text, _callback);
    }
    else {
        uiBtn = &UIManager::instance().addButton(_pos, sz, normalTex, focusedTex, pressedTex, _callback);
    }
}

UIButtonComponent::UIButtonComponent(glm::vec2 _pos, glm::vec2 size, const std::string& _text, sol::function _callback, bool defaultImg)
    : pos(_pos), text(_text), sz(size), useDefaultImg(true)
{
    if (defaultImg) {
        uiBtn->initializeBtnImage("Texture/btn_normal.png", "Texture/btn_focus.png", "Texture/btn_clicked.png");
        uiBtn = &UIManager::instance().addButton(_pos, glm::vec2(200, 100), _text, _callback);
    }
    else {
        uiBtn = &UIManager::instance().addButton(_pos, sz, normalTex, focusedTex, pressedTex, _callback);
    }
}

void UIButtonComponent::setCustomTexture(const std::string& nomalTexPath, const std::string& focusedTexPath, const std::string& pressedTexPath)
{
    normalTex = Texture::create(nomalTexPath);
    focusedTex = Texture::create(focusedTexPath);
    pressedTex = Texture::create(pressedTexPath);

    uiBtn->setCustomTextures(normalTex, focusedTex, pressedTex);

}


void UIButtonComponent::setFullScreen(bool fullscreen)
{
    isFullScreen = fullscreen;

    if (uiBtn && fullscreen) {
        glm::vec2 screenSize = Engine::instance().getWindowSize();

        uiBtn->setPosition(glm::vec2(0, 0));
        uiBtn->setSize(screenSize);

        std::cout << "UIButton set to fullscreen: " << screenSize.x << "x" << screenSize.y << std::endl;
    }
}

void UIButtonComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent
    ("UIButtonComponent",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {
            glm::vec2 _pos;
            _pos.x = args[0];
            _pos.y = args[1];
            const std::string& _text = args[2];
            sol::function _callback = args[3];
            bool defaultImg = args[4];

            return std::make_shared<UIButtonComponent>(_pos, _text, _callback, defaultImg);
        },
        5,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<UIButtonComponent>(comp);
            return sol::make_object(L, derived);
        }
    );
    bindToLua(lua);
}

void UIButtonComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<UIButtonComponent>("UIButtonComponent",
        sol::base_classes, sol::bases<Component>(),
        "setActive", &UIButtonComponent::setActive,
        "setCustomTexture", &UIButtonComponent::setCustomTexture,
        "setFullScreen", &UIButtonComponent::setFullScreen
    );
}
