/*!****************************************************************************
 * file  : SkeletalMeshComponent.cpp
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : October 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include "pch.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ComponentFactory.h"

/**
 * $BRIEF   Constructor - sets all elements within
 *          a skeletal mesh component
 * 
 *          Problem: Animator SHOULD be a separate component
 *          Solution: getting that component from the owner
 *          and binding it in the skeletal mesh component
 * 
 * \param _owner    owner gameobject
 * \param rig_file  skeleton file path
 */
SkeletalMeshComponent::SkeletalMeshComponent(GameObject * _owner, std::string rig_file)
{
    setOwner(_owner);
    setSkeleton(rig_file);
    setAnimator();
    setMaterial();
    setMesh(rig_file);

    clips.clear();
    animator->setSkeleton(skeleton);
    renderObjectInfo = &Renderer::instance().initializeObjects
    (skeletonMesh, SkeletonMaterial, &(owner->getTransform()), renderPassState::SKELETON);
}


/**
 * $BRIEF   Adds animation clip to the animator
 *          These clips will have to be assigned a specific name
 *          this name will be used to access the correct clip
 *          
 * \param animFile  the path to the animation clip file
 * \param name      name of the clip added
 */
void   SkeletalMeshComponent::addAnimClip(std::string animFile, std::string name)
{
   // std::cout << "Adding clip\n";
    int idx = static_cast<int>(clips.size());
    animator->init(animFile);
    clips[name] = idx;
}

void SkeletalMeshComponent::playAnimClip(const std::string& anim_name, bool loop)
{
    auto it = clips.find(anim_name);

    if (it != clips.end())
        animator->play(it->second, loop);
}

/**
 * $BRIEF sets the mesh's material
 */
void SkeletalMeshComponent::setMaterial()
{
    //std::shared_ptr<Shader> SkeletonShader = std::make_shared<Shader>(AssetLoadManager::instance().shader_path_Skeleton);
    std::shared_ptr<Shader> SkeletonShader = std::make_shared<Shader>(AssetLoadManager::instance().shader_path_skeleton_GBuffer);
    SkeletonMaterial = std::make_shared<Material>(SkeletonShader);
}




/**
 * $BRIEF sets the skeletal mesh's animator
 */
void   SkeletalMeshComponent::setAnimator()
{
    animator = std::make_shared<SkeletonAnimator>();
    animator->setSkeleton(skeleton);
}

/**
 * $BRIEF sets the skeleton
 */
void   SkeletalMeshComponent::setSkeleton(std::string rig_file)
{
    skeleton = std::make_shared<mySkeleton>();
    skeleton->init(rig_file);
}

/**
 * $BRIEF sets the mesh for the skeletal mesh object
 */
void SkeletalMeshComponent::setMesh(std::string rig_file)
{
    skeletonMesh = std::make_shared<SkeletonMesh>();
    skeletonMesh->setMaterial(SkeletonMaterial);
    skeletonMesh->m_skeleton = skeleton;
    skeletonMesh->LoadMesh(rig_file);
}

void SkeletalMeshComponent::update()
{
    animator->update(0.01f);
    skeleton->update();

    updateMaterialParameters();
}

void SkeletalMeshComponent::updateMaterialParameters()
{
    
    if (SkeletonMaterial)
    {
        //SkeletonMaterial->setProperty("lightSpaceMatrix", Renderer::instance().getLightSpaceMatrix( ));
        //SkeletonMaterial->setTexture("shadowMap", Renderer::instance().getShadowMap(),1);
        SkeletonMaterial->apply();
    }
    
}

void SkeletalMeshComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent
    ("SkeletalMeshComponent",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {
            std::string mesh = args[0];
            return std::make_shared<SkeletalMeshComponent>(owner, mesh);
        },
        1,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<SkeletalMeshComponent>(comp);
            return sol::make_object(L, derived);
        }
        );
    bindToLua(lua);
}

void SkeletalMeshComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<SkeletalMeshComponent>("SkeletalMeshComponent",
        sol::base_classes, sol::bases<Component>(),
        "addAnimClip", &SkeletalMeshComponent::addAnimClip,
        "playClip", &SkeletalMeshComponent::playAnimClip,
        "setIsVisible", &SkeletalMeshComponent::setIsVisible
    );
        //"makeTransition", &SkeletalMeshComponent::makeTransition
}


