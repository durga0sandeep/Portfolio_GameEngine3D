#include "pch.h"
#include "Graphics/RenderObjectInfo.h"


RenderObjectInfo::RenderObjectInfo()
{


}

bool RenderObjectInfo::shouldRenderInPass(renderPassState pass)
{
    return (renderPass == pass);
}


void RenderObjectInfo::draw(renderPassState pass)
{


}

void RenderObjectInfo::setRenderState(renderPassState pass)
{
    renderPass = pass;
}


renderPassState RenderObjectInfo::getRenderState( )
{
   return  renderPass;
}

void RenderObjectInfo::setIsVisible(bool visibility)
{
    isVisible = visibility;
}

bool RenderObjectInfo::getIsVisible() const
{
    return isVisible;
}


void RenderObjectInfo::addMeshComponent(std::shared_ptr<Mesh> meshComponent)
{

    mesh = meshComponent;

}

void RenderObjectInfo::addMaterialComponent(std::shared_ptr<Material> materialComponent)
{

    material = materialComponent;

}

void RenderObjectInfo::addTransformComponent(Transform* transformComponent)
{
    transform = transformComponent;
}

Material* RenderObjectInfo::getMaterial()
{
    return material.get();
}

Mesh* RenderObjectInfo::getMesh()
{
    return mesh.get();
}

Transform* RenderObjectInfo::getTransform()
{
    return transform;
}












