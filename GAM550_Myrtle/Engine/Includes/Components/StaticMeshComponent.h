/*!****************************************************************************
 * file  : StaticMeshComponent.h
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : November 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once
#include <string>
#include <vector>
#include <functional>
#include "Physics/PhysicsBodyBuilder.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/Renderer.h"
#include "Components/Component.h"
#include "Components/PhysicsComponent.h"
//#include "MeshAsset.h"

class StaticMeshComponent : public Component
{
public:
    StaticMeshComponent(GameObject* _owner ,const std::string& meshPath, const std::string& texturePath);
    ~StaticMeshComponent() {};
    void update() override { }
    void setIsVisible(bool active)
    {
        renderObjectInfo->setIsVisible(active);
    }
    static void    registerWithFactory(sol::state& lua);
    static void    bindToLua(sol::state& lua);

private:
    RenderObjectInfo* renderObjectInfo;
    std::shared_ptr<Mesh>       mesh;
    std::shared_ptr<Material>   material;
    std::shared_ptr<Texture>    texture;
    static std::shared_ptr<Shader> phong_shader;
};
