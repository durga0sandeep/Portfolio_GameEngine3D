#include "pch.h"
#include "Graphics/RenderGraph.h"
#include "Graphics/RenderObjectInfo.h"
#include"Graphics/Mesh.h"
#include"Graphics/Material.h"
#include"Graphics/RenderPass.h"


RenderGraph::RenderGraph( ) :renderQueue(std::make_unique<RenderQueue>()),
    camera(std::make_unique<Camera>()),
    lights() 
{
    // You can also push lights here if needed
    // lights.push_back(Light{});
}



/**
 * $BRIEF
 * add object to render graph with default render state
 * \param mesh
 * \param mat
 * \param trans
 * \return \b 
 */
RenderObjectInfo& RenderGraph::createRenderObjects(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material>  mat, Transform* trans)
{
    RenderObjectInfo RenderObjInf;
    RenderObjInf.addMeshComponent(mesh);
    RenderObjInf.addMaterialComponent(mat);
    RenderObjInf.addTransformComponent(trans);

    return  renderQueue->addOpaqueObjects(RenderObjInf);
}

/**
 * $BRIEF
 * add object to render graph with specific render state
 * \param mesh
 * \param mat
 * \param trans
 * \param state
 * \return \b 
 */
RenderObjectInfo& RenderGraph::createRenderObjects(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material>  mat, Transform* trans,renderPassState state)
{
    RenderObjectInfo RenderObjInf;
    RenderObjInf.addMeshComponent(mesh);
    RenderObjInf.addMaterialComponent(mat);
    RenderObjInf.addTransformComponent(trans);
    RenderObjInf.setRenderState(state);

    return  renderQueue->addOpaqueObjects(RenderObjInf);
}

void RenderGraph::createRenderCamera()
{

}

void RenderGraph::createLights()
{

}

RenderQueue* RenderGraph::getRenderQueue()
{
    return renderQueue.get();
}
