#pragma once
#include "Component.h"
#include <Graphics/Mesh.h>
#include <Graphics/Material.h>
class SpotLightComponent :
    public Component
{
public:

    SpotLightComponent(GameObject* _owner, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 color,
        float radius, float intensity, const std::string& meshPath);

    ~SpotLightComponent() {};

    void update() override {}

    static void    registerWithFactory(sol::state& lua);

    static void bindToLua(sol::state& lua);

    void isMove() const;

    void setPosition(glm::vec3 pos);

    void setScale(glm::vec3 scale);

private:
    int dataRef;

};

