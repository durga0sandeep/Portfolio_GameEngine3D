#pragma once
#include "Component.h"
#include <UI/UIText.h>
#include <UI/UIButton.h>

class ScoreSystemComponent : public Component
{
public:
    ScoreSystemComponent(GameObject* _owner);
    static void registerWithFactory(sol::state& lua);
    static void bindToLua(sol::state& lua);
    void        update() override;
    void        updatePoint(int i);
    int         getPoint();
    void        setPoint(int i);
    void        setColor(int i);
    int         getColor();
private:
    int point = 0;
    UIText* scoreTest;
    UIButton* color;
    int currentColor = 0;
    std::vector<std::shared_ptr<Texture>> allTexture;

};

