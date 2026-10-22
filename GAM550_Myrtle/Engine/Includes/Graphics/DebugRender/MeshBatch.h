/*!****************************************************************************
 * file  : MeshBatch.h
 * brief : Collection of Triangle(Mesh), It just a mesh class
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include <Jolt/Core/Reference.h>
#include <memory>
#include "Graphics/Mesh.h" // Your existing Mesh class

class MeshBatch : public JPH::RefTarget<MeshBatch>, public JPH::RefTargetVirtual
{
public:
    MeshBatch(std::shared_ptr<Mesh> mesh_) : mesh(std::move(mesh_)) {}

    // it is need by Jolt but I never use the ref, using smart pointer
    void AddRef() override {
        ++mRefCount;
    }

    // it is need by Jolt but I never use the ref, using smart pointer
    void Release() override {
        if (--mRefCount == 0)
            delete this;
    }
    std::shared_ptr<Mesh> GetMesh() const { return mesh; }
private:
    std::shared_ptr<Mesh> mesh;
    std::atomic<int> mRefCount = 1;
};

