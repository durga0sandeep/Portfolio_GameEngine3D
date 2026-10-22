#include "pch.h"
#include "Components/UITextComponent.h"
#include "Components/ComponentFactory.h"
#include "Entities/GameObject.h"

UITextComponent::UITextComponent(glm::vec2 _pos, const std::string& _content, float _scale , glm::vec3 _color)
    :pos(_pos), content(_content), textScale(_scale), textColor(_color)
    , uiText(&UIManager::instance().addText(_pos, _content, _scale, _color))
{
}

void UITextComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent
    ("UITextComponent",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {
            glm::vec2 _pos;
            _pos.x = args[0];
            _pos.y = args[1];
            std::string _content = args[2];
            float _scale = args[3];
            glm::vec3 _color = args[4];

            return std::make_shared<UITextComponent>(_pos, _content, _scale , _color);
        },
        5,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<UITextComponent>(comp);
            return sol::make_object(L, derived);
        }
    );
    bindToLua(lua);
}

void UITextComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<UITextComponent>("UITextComponent",
        sol::base_classes, sol::bases<Component>(),
        //"addText", &UITextComponent::addText,
        "setText", &UITextComponent::setText
    );
}
