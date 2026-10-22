#pragma once
#include <vector>
#include "Graphics/RenderObjectInfo.h"

class RenderQueue
{

private:
    std::vector<RenderObjectInfo> transparentQueue;
    std::vector<RenderObjectInfo> opaqueQueue;

public:
    
    RenderQueue();
    RenderObjectInfo& addOpaqueObjects(RenderObjectInfo& OpaqueObj);
    RenderObjectInfo& addTransparentObjects(RenderObjectInfo& transparentObj);

    std::vector<RenderObjectInfo>& getOpaqueQueue();
    std::vector<RenderObjectInfo>& getTransparentQueue();

    void reset();
};

