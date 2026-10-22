#include "pch.h"
#include "Graphics/DebugRender/PhysicDebugRenderer.h"
#include "Graphics/Renderer.h"
#include "Graphics/DebugRender/MeshBatch.h"

/**
 * $BRIEF
 * Physic Debug Renderer constructor, prepare all shader and camera for rendering
 */
PhysicDebugRenderer::PhysicDebugRenderer() :camera(Renderer::instance().camera)
{
    Initialize(); // Required by Jolt
    //line shader setup
    std::shared_ptr<Shader> lineShader = std::make_shared<Shader>("Shader/line_vertex.glsl\nShader/line_fragment.glsl");
    lineMat = std::make_shared<Material>(lineShader);
    //std::shared_ptr<Shader> triShader = std::make_shared<Shader>("Shaders/line_vertex.glsl\nShaders/line_fragment.glsl");
    triangleMat = std::make_shared<Material>(lineShader);
    std::shared_ptr<Shader> debugShader = std::make_shared<Shader>("Shader/debug_vertex.glsl\nShader/debug_fragment.glsl");
    debug_material = std::make_shared<Material>(debugShader);
}

/**
 * $BRIEF
 * helper Draw Line function (not use I think)
 * DrawGeometry call this in the demo, but I have a better way to render so this is not call
 * maybe use in draw velocity(not 100% sure)
 * 
 * \param inFrom
 * \param inTo
 * \param inColor
 */
void PhysicDebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) {
    glm::vec3 from(inFrom.GetX(), inFrom.GetY(), inFrom.GetZ());
    glm::vec3 to(inTo.GetX(), inTo.GetY(), inTo.GetZ());

    std::vector<glm::vec3> vertices = { from, to };

    lineMat->setProperty("uColor", glm::vec3(inColor.r, inColor.g, inColor.b));
    lineMat->setProperty("uView", camera.getView());
    lineMat->setProperty("uProjection", camera.getPerspective());
    lineMat->apply();

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_LINES, 0, 2);
}

/**
 * $BRIEF
 * helper function Draw Triangle for draw geometry
 * not use I think
 * 
 * \param inV1
 * \param inV2
 * \param inV3
 * \param inColor
 * \param inCastShadow
 */
void PhysicDebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow)
{
    // Convert Jolt vectors to glm
    glm::vec3 v1(inV1.GetX(), inV1.GetY(), inV1.GetZ());
    glm::vec3 v2(inV2.GetX(), inV2.GetY(), inV2.GetZ());
    glm::vec3 v3(inV3.GetX(), inV3.GetY(), inV3.GetZ());

    glm::vec3 vertices[] = { v1, v2, v3 };

    triangleMat->setProperty("uColor", glm::vec3(inColor.r, inColor.g, inColor.b));
    triangleMat->setProperty("uView", camera.getView());
    triangleMat->setProperty("uProjection", camera.getPerspective()); 
    triangleMat->apply();

    // Bind VAO and upload vertex data
    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // Draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * $BRIEF
 * To do, need to install freetype to render text
 * 
 * \param inPosition
 * \param inString
 * \param inColor
 * \param inHeight
 */
void PhysicDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor, float inHeight)
{
}

/**
 * $BRIEF
 * DrawGeometry, draw a mesh with debug setting
 * 
 * \param inModelMatrix
 * \param inWorldSpaceBounds
 * \param inLODScaleSq
 * \param inModelColor
 * \param inGeometry
 * \param inCullMode
 * \param inCastShadow
 * \param inDrawMode
 */
void PhysicDebugRenderer::DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode = ECullMode::CullBackFace, ECastShadow inCastShadow = ECastShadow::On, EDrawMode inDrawMode = EDrawMode::Wireframe)
{
    // https://github.com/jrouwe/JoltPhysics/discussions/902
    // Cast the Batch back to MeshBatch
    const LOD* lod = inGeometry->mLODs.data();

    const MeshBatch* meshBatch = static_cast<const MeshBatch*>(lod->mTriangleBatch.GetPtr());
    std::shared_ptr<Mesh> mesh = meshBatch->GetMesh();

    
    // Convert Jolt matrix to glm::mat4
    glm::mat4 modelMatrix;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            modelMatrix[i][j] = inModelMatrix(j, i); // Note: transposed due to column-major

    // Convert Jolt color to glm::vec4
    JPH::Color color = inModelColor;
    glm::vec4 debugColor(
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    );

    // Set material properties
    debug_material->setProperty("uColor", glm::vec3(debugColor.r, debugColor.g, debugColor.b));
    debug_material->setProperty("uView", camera.getView());
    debug_material->setProperty("uProjection", camera.getPerspective());
    debug_material->setProperty("uModel", modelMatrix);
    debug_material->apply();

    // Handle cull mode
    if (inCullMode == ECullMode::CullBackFace) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else if (inCullMode == ECullMode::CullFrontFace) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }
    else {
        glDisable(GL_CULL_FACE);
    }

    // Handle draw mode
    GLenum drawMode = GL_FILL;
    if (inDrawMode == EDrawMode::Wireframe) {
        drawMode = GL_LINE;
    }
    glPolygonMode(GL_FRONT_AND_BACK, drawMode);

    // Bind and draw
    mesh->getGeometryBuffer()->bind();

    if (mesh->hasAttribute(GeometryBuffer::Attribute::Position))
    {
        if (mesh->getGeometryBuffer()->getIndexCount() > 0)
        {
            glDrawElements(GL_TRIANGLES, mesh->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, mesh->getGeometryBuffer()->getVertexCount());
        }
    }

    mesh->getGeometryBuffer()->unbind();

    // Restore default state
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}


/**
 * $BRIEF
 * Set up GPU Resource that need, need to run after renderer setup(glset current context)
 * 
 */
void PhysicDebugRenderer::setupResource()
{
    //Generate and bind VAO and VBO
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

    //Define vertex layout (position only)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    //Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

    // Define vertex layout
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    // Done configuring
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * $BRIEF
 * Create Triangle Batch(mesh), it just a set of triangle
 * \param inTriangles
 * \param inTriangleCount
 * \return \b 
 */
JPH::DebugRenderer::Batch PhysicDebugRenderer::CreateTriangleBatch(
    const JPH::DebugRenderer::Triangle* inTriangles,
    int inTriangleCount)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < inTriangleCount; ++i) {
        const auto& tri = inTriangles[i];

        for (int j = 0; j < 3; ++j) {
            const auto& pos = tri.mV[j].mPosition;
            const auto& normal = tri.mV[j].mNormal;
            const auto& uv = tri.mV[j].mUV;

            vertices.push_back(pos[0]); // x
            vertices.push_back(pos[1]); // y
            vertices.push_back(pos[2]); // z

            vertices.push_back(normal[0]);
            vertices.push_back(normal[1]);
            vertices.push_back(normal[2]);

            vertices.push_back(uv.x);
            vertices.push_back(uv.y);

            indices.push_back(i * 3 + j);
        }
    }

    auto mesh = std::make_shared<Mesh>("JoltDebugTriangleBatch");
    mesh->setVertexData({
        {GeometryBuffer::Attribute::Position, vertices, 3},
        {GeometryBuffer::Attribute::Normal, vertices, 3},
        {GeometryBuffer::Attribute::TexCoord, vertices, 2}
        }, 8 * sizeof(float), indices);
    return new MeshBatch(mesh);
}

/**
 * $BRIEF
 * Create Triangle Batch(mesh), it just a set of triangle vertex, this one with indices
 * \param inTriangles
 * \param inTriangleCount
 * \return \b
 */
JPH::DebugRenderer::Batch PhysicDebugRenderer::CreateTriangleBatch(
    const JPH::DebugRenderer::Vertex* inVertices,
    int inVertexCount,
    const JPH::uint32* inIndices,
    int inIndexCount)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < inVertexCount; ++i)
    {
        const auto& vert = inVertices[i];

        // Position
        vertices.push_back(vert.mPosition[0]);
        vertices.push_back(vert.mPosition[1]);
        vertices.push_back(vert.mPosition[2]);

        // Normal
        vertices.push_back(vert.mNormal[0]);
        vertices.push_back(vert.mNormal[1]);
        vertices.push_back(vert.mNormal[2]);

        // UV
        vertices.push_back(vert.mUV.x);
        vertices.push_back(vert.mUV.y);
    }

    // Copy indices directly
    indices.insert(indices.end(), inIndices, inIndices + inIndexCount);

    auto mesh = std::make_shared<Mesh>("JoltDebugIndexedBatch");
    mesh->setVertexData({
        {GeometryBuffer::Attribute::Position, vertices, 3},
        {GeometryBuffer::Attribute::Normal, vertices, 3},
        {GeometryBuffer::Attribute::TexCoord, vertices, 2}
        }, 8 * sizeof(float), indices);

    return new MeshBatch(mesh);
}




