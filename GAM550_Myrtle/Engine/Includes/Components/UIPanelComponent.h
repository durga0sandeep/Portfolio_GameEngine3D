#pragma once

#include "Components/Component.h"
#include "UI/UIManager.h"

class UIPanelComponent : public Component
{
public:
    UIPanelComponent(glm::vec2 _pos, glm::vec2 _size, std::string _texturePath);
    ~UIPanelComponent() {};

    static void registerWithFactory(sol::state& lua);
    static void bindToLua(sol::state& lua);

    void setActive(bool _active)
    {
        uiPanel->setActive(_active);
    }

private:
    UIPanel* uiPanel = nullptr;
    glm::vec2 pos;
    glm::vec2 size;
    std::string texturePath;
};

