/*!****************************************************************************
 * file  : StaticMeshComponent.cpp
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include "pch.h"
#include "Components/StaticMeshComponent.h"
#include "Memory/AssetLoadManager.h"
#include "Entities/GameObject.h"
#include "Components/ComponentFactory.h"

std::shared_ptr<Shader> StaticMeshComponent::phong_shader = nullptr;

StaticMeshComponent::StaticMeshComponent(GameObject* _owner, const std::string& meshPath, const std::string& texturePath)
{
    setOwner(_owner);
    auto& asset_manager = AssetLoadManager::instance();
    if (!phong_shader) {
        //phong_shader = std::make_shared<Shader>(asset_manager.shader_path_PhongLight);
        phong_shader = std::make_shared<Shader>(asset_manager.shader_path_GBuffer);
    }
    material = std::make_shared<Material>(phong_shader);
    mesh = asset_manager.setAssetMesh(meshPath.c_str());
    texture = asset_manager.assetTexture(texturePath.c_str());

    material->setTexture("mainTexture", texture, 0);

    renderObjectInfo = &Renderer::instance().initializeObjects(mesh, material, &(owner->getTransform()), renderPassState::DEFERRED);
    
    if (texturePath != ""){
        std::cout << texturePath << "\n\n\n";
    }
    else{
        std::cout << "texture is empty!!!!!!!!!!!!!\n";
    }
    
    //Renderer::instance().initializeObjects(mesh, material, &(owner->getTransform()));

}

void StaticMeshComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent
    ("StaticMeshComponent",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {
            std::string mesh = args[0];
            std::string texture = args[1];
            return std::make_shared<StaticMeshComponent>(owner, mesh, texture);
        },
        2,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<StaticMeshComponent>(comp);
            return sol::make_object(L, derived);
        });
    bindToLua(lua);
}

void StaticMeshComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<StaticMeshComponent>("StaticMeshComponent",
        sol::base_classes, sol::bases<Component>(),
        // Bind your own methods
        "setIsVisible", &StaticMeshComponent::setIsVisible
    );
}
