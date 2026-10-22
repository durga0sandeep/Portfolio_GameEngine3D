#pragma once
#include "Component.h"
#include "Graphics/Renderer.h"
#include "Entities/GameObject.h"
#include "Input/Input.h"

class InputComponenet : public Component
{
public:
    InputComponenet(GameObject* _owner);
    static void registerWithFactory(sol::state& lua);
    static void bindToLua(sol::state& lua);
    void        update() override;
    bool        isButtonClick(std::string button);
};

