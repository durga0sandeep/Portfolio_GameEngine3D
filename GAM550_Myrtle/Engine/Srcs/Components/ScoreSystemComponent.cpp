#include "pch.h"
#include "Components/ComponentFactory.h"
#include "Components/ScoreSystemComponent.h"
#include <UI/UIManager.h>

ScoreSystemComponent::ScoreSystemComponent(GameObject* _owner)
    :scoreTest(&UIManager::instance().addText(glm::vec2(40, 50), "SCORE : 0", 1, glm::vec3(1.0, 1.0, 0))),
    color(&UIManager::instance().addButton(glm::vec2(500, 100), glm::vec2(50, 50),
        AssetLoadManager::instance().assetTexture("Game/Assets/Textures/red.png")
        , AssetLoadManager::instance().assetTexture("Game/Assets/Textures/red.png")
        , AssetLoadManager::instance().assetTexture("Game/Assets/Textures/red.png"), sol::nil))

{
    allTexture.push_back(AssetLoadManager::instance().assetTexture("Game/Assets/Textures/red.png"));
    allTexture.push_back(AssetLoadManager::instance().assetTexture("Game/Assets/Textures/blue.png"));
}

void ScoreSystemComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent(
        "ScoreSystemComponent",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {
            return std::make_shared<ScoreSystemComponent>(owner);
        },
        0,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<ScoreSystemComponent>(comp);
            return sol::make_object(L, derived);
        }
    );
    bindToLua(lua);
}

void ScoreSystemComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<ScoreSystemComponent>("ScoreSystemComponent",
        sol::base_classes, sol::bases<Component>(),
        // Bind your own methods
        "updatePoint", &ScoreSystemComponent::updatePoint,
        "getPoint", &ScoreSystemComponent::getPoint,
        "setPoint", &ScoreSystemComponent::setPoint,
        "setColor", &ScoreSystemComponent::setColor,
        "getColor", &ScoreSystemComponent::getColor
    );
}

void ScoreSystemComponent::update()
{
    // do nothing
}

void ScoreSystemComponent::updatePoint(int i)
{
    point += i;
    scoreTest->setText("Score : " + std::to_string(point));
}

int ScoreSystemComponent::getPoint()
{
    return point;
}

void ScoreSystemComponent::setPoint(int i)
{
    point = i;
    scoreTest->setText("Score : " + std::to_string(point));
}

void ScoreSystemComponent::setColor(int i)
{
    currentColor = i;
    color->texture = allTexture[i];
}

int ScoreSystemComponent::getColor()
{
    return currentColor;
}
