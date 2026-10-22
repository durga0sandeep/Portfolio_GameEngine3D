#include "pch.h"
#include "Components/UIPanelComponent.h"
#include "Components/ComponentFactory.h"
#include "Entities/GameObject.h"

UIPanelComponent::UIPanelComponent(glm::vec2 _pos, glm::vec2 _size, std::string _texturePath)
    :pos(_pos), size(_size), texturePath(_texturePath)
{
    uiPanel = &UIManager::instance().addPanel(_pos, _size, _texturePath);
}

void UIPanelComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent
    ("UIPanelComponent",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {
            glm::vec2 _pos;
            _pos.x = args[0];
            _pos.y = args[1];

            glm::vec2 _sz;
            _sz.x = args[2];
            _sz.y = args[3];

            std::string texPath = args[4];

            return std::make_shared<UIPanelComponent>(_pos, _sz, texPath);
        },
        5,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<UIPanelComponent>(comp);
            return sol::make_object(L, derived);
        }
    );
    bindToLua(lua);
}

void UIPanelComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<UIPanelComponent>("UIPanelComponent",
        sol::base_classes, sol::bases<Component>(),
        "setActive", &UIPanelComponent::setActive
        /*,

        "addText", & UIPanelComponent::addText*/
    );
}
