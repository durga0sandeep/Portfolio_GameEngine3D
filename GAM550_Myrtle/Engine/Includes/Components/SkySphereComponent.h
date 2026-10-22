/*!****************************************************************************
 * file  : SkySphereComponent.h
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once

#include "Component.h"
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Entities/GameObject.h"

class SkySphereComponent : public Component
{
private:
    std::shared_ptr<Mesh>       skyMesh;
    std::shared_ptr<Shader>     skyShader;
    std::shared_ptr<Material>   skyMaterial;
    std::shared_ptr<Texture>    skyTexture;

public:
    SkySphereComponent
        (GameObject* _owner,
        std::string    mesh,
        std::string    texture)
    {
        setOwner(_owner);
        skyShader = std::make_shared<Shader>(AssetLoadManager::instance().shader_path_Skydome);
        skyMaterial = std::make_shared<Material>(skyShader);
        skyTexture = AssetLoadManager::instance().loadTextureImage(texture);
        skyMesh = AssetLoadManager::instance().setAssetMesh(mesh.c_str());
        owner->getTransform().setScale(glm::vec3(8000));
        skyMaterial->setTexture("mainTexture", skyTexture, 0);
        Renderer::instance().initializeObjects(skyMesh, skyMaterial, &(owner->getTransform()),renderPassState::SKYDOME);
        //std::cout << "skysphere created!\n";
    }

    static void    registerWithFactory();

    ~SkySphereComponent() {};

    void update() override
    {
        owner->getTransform().setPosition(Renderer::instance().camera.getEye());
    }
};
