#include "pch.h"
#include "Components/InputComponenet.h"
#include "Components/ComponentFactory.h"

InputComponenet::InputComponenet(GameObject* _owner)
{
    setOwner(_owner);
}

void InputComponenet::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent(
        "InputComponenet",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {
            return std::make_shared<InputComponenet>(owner);
        },
        1,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<InputComponenet>(comp);
            return sol::make_object(L, derived);
        }
    );
    bindToLua(lua);
}

void InputComponenet::bindToLua(sol::state& lua)
{
    lua.new_usertype<InputComponenet>("InputComponenet",
        sol::base_classes, sol::bases<Component>(),
        "isButtonClick", &InputComponenet::isButtonClick
    );
}

void InputComponenet::update()
{
}

bool InputComponenet::isButtonClick(std::string button)
{
    if (button == "m") {
        return Input::instance().IsKeyPressed(KeyCode::M);
    }
    if (button == "up") {
        return Input::instance().IsKeyPressed(KeyCode::Up);
    }
    if (button == "down") {
        return Input::instance().IsKeyPressed(KeyCode::Down);
    }
    if (button == "enter") {
        return Input::instance().IsKeyPressed(KeyCode::Enter);
    }
}
