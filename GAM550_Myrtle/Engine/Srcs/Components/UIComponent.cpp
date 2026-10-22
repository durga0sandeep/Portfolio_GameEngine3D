#include "pch.h"
//#include "Components/UIComponent.h"
//#include <UI/UIManager.h>
//#include <Audio/AudioManager.h>
//#include "Components/ComponentFactory.h"
//
//// Main constructor that auto-determines type based on texture path
////UIComponent::UIComponent(GameObject* _owner, const std::string& _texturePath,
////    glm::vec2 _pos, glm::vec2 _size, UIType _type)
////    : Component(_owner), texturePath(_texturePath), pos(_pos), size(_size),
////    type(_type), built(false), useDefaultImg(false)
////{
////    // Auto-build based on type
////    //build();
////}
//
//// Legacy numeric callback constructor (keeping for backward compatibility)
////UIComponent::UIComponent(GameObject* _owner, const std::string& texturePath,
////    glm::vec2 pos, glm::vec2 size, int num)
////    : Component(_owner), texturePath(texturePath), pos(pos), size(size),
////    type(UIType::BUTTON), built(false)
////{
////    setOwner(_owner);
////
////    // Create button with predefined callbacks
////    std::function<void()> callback;
////    switch (num) {
////    case 0: callback = []() { AudioManager::instance().stopBGM(); }; break;
////    case 1: callback = []() { AudioManager::instance().bgmVolumeUp(); }; break;
////    case 2: callback = []() { AudioManager::instance().bgmVolumeDown(); }; break;
////    case 3: callback = []() { AudioManager::instance().toggleSFXPause(); }; break;
////    case 4: callback = []() { AudioManager::instance().sfxVolumeUp(); }; break;
////    case 5: callback = []() { AudioManager::instance().sfxVolumeDown(); }; break;
////    default: callback = nullptr; break;
////    }
////
////    UIManager::instance().addButton(pos, size, texturePath, callback);
////}
//
//// Text/Panel constructor
////UIComponent::UIComponent(GameObject* owner, UIType _type, float x, float y,
////    float w, float h, const std::string& _data)
////    : Component(owner), type(_type), pos(x, y), size(w, h),
////    data(_data), built(false), useDefaultImg(false)
////{
////    build();
////}
//
//UIComponent(GameObject* owner, UIType _type, float x, float y,
//    float w, float h)
//    : Component(owner), type(_type), pos(x, y), size(w, h),
//    data(_data), built(false), useDefaultImg(false) {};
//
//UIComponent::~UIComponent()
//{
//    if (luaOnClick.valid())
//        luaOnClick.abandon();
//}
//
////void UIComponent::useDefaultTextures()
////{
////    useDefaultImg = true;
////}
//
////void UIComponent::setOnClick(sol::function callback)
////{
////    luaCallback = callback;
////
////    // If button already built, update its callback
////    if (built && button) {
////        std::function<void()> wrappedCallback = [this]() {
////            try {
////                if (luaCallback.valid()) {
////                    luaCallback();
////                }
////            }
////            catch (const sol::error& e) {
////                std::cerr << "Lua callback error: " << e.what() << std::endl;
////            }
////            };
////        button->setCallback(wrappedCallback);
////    }
////}
//
//
//void UIComponent::onClick()
//{
//    std::cout << "trigger called\n";
//    if (luaOnClick.valid())
//    {
//        luaOnClick(*this);
//    }
//}
//
//void UIComponent::setTextColor(float r, float g, float b)
//{
//    textColor = glm::vec3(r, g, b);
//}
//
//void UIComponent::setTextScale(float scale)
//{
//    textScale = scale;
//}
//
//void UIComponent::setLuaOnClick(sol::function func)
//{
//    luaOnClick = func;
//}
//
//void UIComponent::setPos(float x, float y)
//{
//    pos = glm::vec2(x, y);
//}
//
//void UIComponent::setIsActive(bool active)
//{
//    isActive = active;
//}
//
//bool UIComponent::getIsActive()
//{
//    return isActive;
//}
//
////void UIComponent::setTextColor(float r, float g, float b)
////{
////    textColor = glm::vec3(r, g, b);
////
////    // Update existing text if already built
////    if (built && uiText) {
////        uiText->setColor(textColor);
////    }
////}
////
////void UIComponent::setTextScale(float scale)
////{
////    textScale = scale;
////
////    // Update existing text if already built
////    if (built && uiText) {
////        uiText->setScale(textScale);
////    }
////}
//
////void UIComponent::build()
////{
////    if (built) return;
////
////    switch (type) {
////    case UIType::BUTTON:
////    {
////        // Create callback wrapper
////        std::function<void()> callback = nullptr;
////        if (luaCallback.valid()) {
////            callback = [this]() {
////                try {
////                    luaCallback();
////                }
////                catch (const sol::error& e) {
////                    std::cerr << "Lua callback error: " << e.what() << std::endl;
////                }
////                };
////        }
////
////        // Use texture path if provided, otherwise use text data
////        if (!texturePath.empty()) {
////            auto tex = AssetLoadManager::instance().assetTexture(texturePath.c_str());
////            button = std::make_shared<UIButton>(pos, size, tex, callback);
////        }
////        else {
////            button = std::make_shared<UIButton>(pos, data, callback);
////        }
////
////        // Set button to use default textures if requested
////        /*if (useDefaultImg) {
////            button->setUseDefaultImage(true);
////        }*/
////
////        // Add to UIManager with page name
////        UIManager::instance().addButton(button, pageName);
////    }
////    break;
////
////    case UIType::TEXT:
////    {
////        uiText = std::make_shared<UIText>(pos, data, textScale, textColor);
////        UIManager::instance().addText(uiText, pageName);
////    }
////    break;
////
////    case UIType::PANEL:
////    {
////        panel = std::make_shared<UIPanel>(pos, size);
////        if (!texturePath.empty()) {
////            auto tex = AssetLoadManager::instance().assetTexture(texturePath.c_str());
////            panel->setTexture(tex);
////        }
////        UIManager::instance().addPanel(panel, pageName);
////    }
////    break;
////    }
////
////    built = true;
////}
//
//void UIComponent::bindToLua(sol::state& lua)
//{
//    lua.new_usertype<UIComponent>("UIComponent",
//        sol::base_classes, sol::bases<Component>(),
//        "setTextColor", &UIComponent::setTextColor,
//        "setTextScale", &UIComponent::setTextScale
//        
//    );
//}
//
//void UIComponent::registerWithFactory(sol::state& lua)
//{
//    ComponentFactory::instance().registerComponent(
//        "UIComponent",
//        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
//        {
//            // Args: texturePath (optional), maxParticles (optional), autoEmit (optional)
//            std::string _name = args[0];
//            UIType _type = args[1];
//            float x_pos = args[2];
//            float y_pos = args[3];
//            float width = args[4];
//            float height = args[5];
//
//            return std::make_shared<UIComponent>(owner, _name, _type, x_pos, y_pos, width, height);
//        },
//        6,
//        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
//        {
//            auto derived = std::dynamic_pointer_cast<UIComponent>(comp);
//            return sol::make_object(L, derived);
//        }
//    );
//
//    bindToLua(lua);
//}
