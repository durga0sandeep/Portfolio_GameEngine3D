/*!****************************************************************************
 * file  : SkeletalMeshComponent.h
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once
#include <map>
#include "Component.h"
#include <Graphics/SkeletalAnimation/mySkeleton.h>
#include <Graphics/SkeletalAnimation/SkeletonAnimator.h>
#include <Graphics/SkeletalAnimation/SkeletonMesh.h>
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Memory/AssetLoadManager.h"
#include "Entities/GameObject.h"

class SkeletalMeshComponent : public Component
{
private:
    std::shared_ptr<mySkeleton>         skeleton = nullptr;
    std::shared_ptr<SkeletonAnimator>   animator = nullptr;
    std::shared_ptr<SkeletonMesh>       skeletonMesh;
    std::shared_ptr<Material>           SkeletonMaterial;
    Transform                           skeletonTransform;
    std::unordered_map<std::string, int> clips;
    RenderObjectInfo* renderObjectInfo;

public:
    SkeletalMeshComponent(GameObject* _owner, std::string rig_file);

    ////////////////////////////
    /// mesh methods:
    void    setSkeleton(std::string rig_file);
    void    setMaterial();
    void    setMesh(std::string rig_file);

    ////////////////////////////
    /// Animator methods:
    void    setAnimator();
    void    addAnimClip(std::string animFile, std::string name);
    void    playAnimClip(const std::string& anim_name, bool loop);
    void    setIsVisible(bool active) {
        renderObjectInfo->setIsVisible(active);
    };

    static void    registerWithFactory(sol::state& lua);

    static void bindToLua(sol::state& lua);

    void    update() override;
    void updateMaterialParameters();
};
