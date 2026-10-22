/*!****************************************************************************
 * file  : PhysicDebugRenderer.h
 * brief : Debug Renderer for physic box and velocity, 
 * it uses Jolt Physic Bebug Renderer
 * with opengl rendering implemented
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/Material.h>
#include <memory>
#include <Graphics/Mesh.h>
#include <Entities/Camera.h>
#include <Jolt/Core/Core.h>
#include <Jolt/Renderer/DebugRenderer.h>

class PhysicDebugRenderer : public JPH::DebugRenderer {
public:
    PhysicDebugRenderer();

    virtual ~PhysicDebugRenderer() = default;

    // Draw a line between two points
    void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override;

    // Draw a triangle
    void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow = ECastShadow::Off) override;

    // Optional: 3D text rendering (not implemented)
    void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor = JPH::Color::sWhite, float inHeight = 0.5f) override;

    void DrawGeometry(JPH::RMat44Arg, const JPH::AABox&, float, JPH::ColorArg, const GeometryRef&, ECullMode, ECastShadow, EDrawMode) override;

    void setupResource();

    JPH::DebugRenderer::Batch CreateTriangleBatch(const JPH::DebugRenderer::Triangle* inTriangles, int inTriangleCount) override;
    JPH::DebugRenderer::Batch CreateTriangleBatch(const JPH::DebugRenderer::Vertex* inVertices, int inVertexCount, const JPH::uint32* inIndices, int inIndexCount) override;
private:
    Camera& camera;
    std::shared_ptr<Material> lineMat; // Assume Shader is a class that handles OpenGL shader compilation and usage
    std::shared_ptr<Material> triangleMat;
    std::shared_ptr<Material> debug_material;
    GLuint lineVAO, lineVBO, triangleVAO, triangleVBO;

};

