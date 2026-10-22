#include "pch.h"
#include "Graphics/RenderQueue.h"

RenderQueue::RenderQueue()
{
    opaqueQueue.reserve(1000);
}

RenderObjectInfo& RenderQueue::addOpaqueObjects(RenderObjectInfo& OpaqueObj)
{
    opaqueQueue.push_back(OpaqueObj);

    return opaqueQueue.back();
}

RenderObjectInfo& RenderQueue::addTransparentObjects(RenderObjectInfo& transparentObj)
{
    transparentQueue.push_back(transparentObj);

    return transparentQueue.back();

}

std::vector<RenderObjectInfo>& RenderQueue::getOpaqueQueue()
{
    // TODO: insert return statement here
    return opaqueQueue;
}

std::vector<RenderObjectInfo>& RenderQueue::getTransparentQueue()
{
    // TODO: insert return statement here
    return transparentQueue;
}

void RenderQueue::reset()
{
    opaqueQueue.clear();
    transparentQueue.clear();
}

