#pragma once

#include "Components/Transform.h"
#include"Graphics/Mesh.h"
#include"Graphics/Material.h"
#include"Graphics/RenderPass.h"
#include"Graphics/RenderPass.h"
#include<memory.h>



class RenderObjectInfo
{

    public: 
        RenderObjectInfo();
        bool shouldRenderInPass(renderPassState pass);
        void draw(renderPassState pass);

        void setRenderState(renderPassState pass);
        void addMeshComponent(std::shared_ptr<Mesh> meshComponent);
        void addMaterialComponent(std::shared_ptr<Material> materialComponent);
        void addTransformComponent(Transform* transformComponent);
        Material* getMaterial();
        Mesh* getMesh();
        Transform* getTransform();

        renderPassState getRenderState();

        void setIsVisible(bool visibility);
        bool getIsVisible() const;

    private:
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
        Transform* transform;

        renderPassState renderPass = renderPassState::BASE;

        bool castsShadow;
        bool affectsReflection;
        bool isVisible = true;


};





