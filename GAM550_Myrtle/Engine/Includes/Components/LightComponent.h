/*!****************************************************************************
 * file  : LightComponent.h
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <functional>
#include "Components/Component.h"
#include <glm/vec3.hpp> // For glm::vec3
#include "Graphics/Renderer.h"


/*!****************************************************************************
* 
* Light component:
*   type of light (Directional... etc)
*   Color
*   Mode (baked, real time.. etc)
*   Intensity
*   Indirect Multiplyer 
*   Shadows (does it cause shadows)
*   Draw Halo (bool)
*   Render mode
*   culling mask
******************************************************************************/
class LightComponent : public Component
{
public:

    LightComponent(GameObject* _owner, glm::vec3 position, glm::vec3 color,
        float radius, float intensity, const std::string& meshPath);

    ~LightComponent() {};

    void update() override {}

    static void    registerWithFactory(sol::state& lua);

    static void bindToLua(sol::state& lua);

    void isMove() const;

    void setPosition(glm::vec3 pos);

    void setScale(glm::vec3 scale);

private:
    std::shared_ptr<Mesh>       mesh;
    std::shared_ptr<Material>   material;
    std::shared_ptr<Texture>    texture;
    static std::shared_ptr<Shader> phong_shader;
    int dataRef;

};



