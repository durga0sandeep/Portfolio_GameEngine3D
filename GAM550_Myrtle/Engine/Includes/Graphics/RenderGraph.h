#pragma once

#include "Graphics/RenderQueue.h"
#include "Graphics/Light.h"
#include "Entities/Camera.h"
#include <vector>
#include <memory>

class RenderGraph
{
    private:

        std::unique_ptr<RenderQueue> renderQueue;
        std::unique_ptr<Camera> camera;
        std::vector<Light> lights;


    public:

        RenderGraph();
        RenderObjectInfo& createRenderObjects(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material>  mat, Transform* trans);
        RenderObjectInfo& createRenderObjects(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, Transform* trans, renderPassState state);
        void createRenderCamera();
        void createLights();
        RenderQueue* getRenderQueue();


};







