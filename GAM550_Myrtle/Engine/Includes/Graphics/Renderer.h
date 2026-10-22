/*!****************************************************************************
 * file  : Renderer.h
 * brief : Renderer - The Renderer class provides functions to initialize rendering,
 * clear the screen, draw objects (indexed or non-indexed),
 * and manage the main rendering loop.
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once

#include "pch.h"
#include "Graphics/GameWindow.h"
#include "Graphics/Shader.h"
#include "Graphics/Light.h"
#include "Entities/Camera.h"
#include "Graphics/GeometryBuffer.h"
#include "Graphics/Mesh.h"
#include "Components/Transform.h"
#include "Graphics/Renderer.h"
#include "Memory/AssetLoadManager.h"
#include "Graphics/RenderQueue.h"
#include "Graphics/RenderObjectInfo.h"
#include "Graphics/RenderGraph.h"
#include "Graphics/Fbo.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/CreateTexture.h"
#include "Graphics/msmHelper.h"
#include "Graphics/Quad.h"
#include "Graphics/LocalLight.h"
#include "Graphics/SSBO.h"
#include "Graphics/SphereClass.h"
#include "Graphics/ParticleSystem/ParticleSystemManager.h"

/**
 * @struct RenderContainer
 * @brief Container for rendering data.
 *
 * Holds references to a Mesh, its GeometryBuffer, and the Transform
 * for a single renderable object.
 */
struct RenderContainer
{
    std::shared_ptr<Mesh> meshPtr;              /**< Pointer to the mesh. */
    std::shared_ptr<GeometryBuffer> geomBuffer; /**< Pointer to the geometry buffer. */
    Transform transform;                        /**< Transform of the object (position, rotation, scale). */
};



/**
 * @class Renderer
 * @brief Handles rendering objects and UI to the game window.
 *
 * The Renderer class provides functions to initialize rendering,
 * clear the screen, draw objects (indexed or non-indexed),
 * and manage the main rendering loop.
 */
class Renderer {

public:

    //Singleton
    static Renderer& instance();

    // Delete copy constructor and assignment operator
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    /** @brief Clears the screen with the given RGBA color. */
    void clear(float r, float g, float b, float a);

    /**
     * @brief Draws geometry.
     * @param dataCount Number of vertices or indices to draw.
     * @param drawingMode OpenGL drawing mode (e.g., GL_TRIANGLES).
     * @param indexed True if drawing using an index buffer.
     */
    void draw(unsigned int dataCount, GLenum drawingMode, bool indexed);

    /**
     * @brief Draws UI elements.
     * @param dataCount Number of vertices or indices.
     * @param drawingMode OpenGL drawing mode.
     * @param indexed True if using index buffer.
     */
    void drawUI(unsigned int dataCount, GLenum drawingMode, bool indexed);

    /** @brief Initializes the renderer and sets up OpenGL state. */
    void initialize(GameWindow* window);

    /** @brief Shuts down the renderer and releases resources. */
    void shutdown();

    /** @brief Runs the main rendering loop. */
    void run();

    /**
     * @brief Renders a list of objects with the given shader and camera.
     * @param Shaders Pointer to the Shader to use.
     * @param Cam Pointer to the Camera.
     * @param window Pointer to the GLFW window.
     * @param container Vector of RenderContainer holding objects to render.
     */
    void render(Shader* Shaders, GLFWwindow* window, std::vector<RenderContainer>& container);

    void render(GLFWwindow* window);

    // 1. Geometry Pass: render scene's geometry/color data to G-buffer
    void geometryPass();

    // 1.1 Shadow Pass: renders shadow
    //void setupShadowMap();
    //void setUpShadow();
    //void shadowPass();

    //Compute Shaders
    void horizVerticalCS();

    // 2. Lighting Pass: use G-buffer to calculate lighting
    void lightingPass();

    // 3. Many Local lights 
    void manyLocalLightsPass();

    void manySpotLightsPass();

    // 4. Post-processing or final pass (optional)
    void postProcessingPass();

    void initializeRenderObjects();

    void initDeferredRendering();

    void setupShadowMap();

    RenderGraph& getRenderGraph();

    Camera camera;

    RenderObjectInfo& initializeObjects(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material>  mat, Transform* trans);

    RenderObjectInfo& initializeObjects(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, Transform* trans, renderPassState state);

    msmHelper msmMethods;

    // Return a scale matrix
    glm::mat4 scale(const float x, const float y, const float z);
    // Return a translation matrix
    glm::mat4 translate(const float x, const float y, const float z);


    LocalLight pointLightScript;
    SSBO ssbo;

    LocalLight spotLightScript;
    SSBO ssboSpotLight;
    int creatingPointLights(glm::vec3 pos, glm::vec3 color, float intensity, float radius, const std::string& meshPath);
    int creatingSpotLights(glm::vec3 pos, glm::vec3 color, float intensity, float radius, const std::string& meshPath);
    LocalLight& getLocalLightList() { return pointLightScript; }
    LocalLight& getSpotLightList() { return spotLightScript; }

    ParticleSystemManager* particlesys;

    void setDebugMode(bool val) { debug = val; }
    bool getDebugMode() { return debug; }

    std::shared_ptr<Texture> getShadowMap() const;
    glm::mat4 getLightSpaceMatrix() const;

private:

    Light light;
    float distLightToOrigin;
    float zNear;
    float zFar;
    // for shadow map
    float halfSize = 20.0f;

    float newFrustumSize = 400.0f; 
    float nearPlane = 1.0f;       
    float farPlane = 100.0f;

    std::vector<glm::vec3> colors;

    Shader shader;
    RenderGraph renderGraph;

    //void normalRender(RenderObjectInfo);
    //void skydomeRender(RenderObjectInfo);
    void deferredRendering();
    GLuint shadowFBO;

    std::shared_ptr<Texture> shadowmap;
    std::shared_ptr<Material> depthMat;
    std::shared_ptr<Material> depthskeletonMat;
    glm::mat4 lightSpaceMatrix;
   

    glm::mat4 lightProjection;
    glm::mat4 lightView;


    //general purpose
    const int SHADOW_WIDTH = 4096;
    

    int width, height;

    void normalRender(RenderObjectInfo);
    void generateShadowMap();
    void SkeletonRender(RenderObjectInfo RenderObject);
    void SkeletonShadowRender(RenderObjectInfo RenderObject);
    void skydomeRender(RenderObjectInfo);

    bool debug = false;

    unsigned int gBuffer, gPosition, gNormal, gAlbedoSpec;
   
    unsigned int shadowFboId;
    unsigned int depthBuffer;
    unsigned int shadowTextureID;

    enum class gBufferTextureType {
        Position,
        Normal,
        AlbedoSpec,
        Count // Always keep this last
    };
    std::vector< std::shared_ptr<Texture>> gbufferTextures;
    Texture ShadowFbo;
    int screenWidth = 800;
    int screenHeight = 600;

    GLuint rboDepth;

    std::shared_ptr<Mesh> sphereMesh1;

    std::shared_ptr<Material> lightingMaterial;
    std::shared_ptr<Material> shadowMaterial;
    std::shared_ptr<Material> pointlightMaterial;
    std::shared_ptr<Material> spotlightMaterial;

    std::shared_ptr<SphereClass> sphereMesh;
    std::shared_ptr<Quad> fullscreenQuad;//used for defferred rendering


    /*
    std::shared_ptr<Mesh> SphereMesh;
    std::shared_ptr<Transform> SphereTransform;
    std::shared_ptr<RenderObjectInfo> SphereRenderObj;
    */

    
    int programId, shadowPrgId, gBufferPrgId, localLightsPrgId, horzPrjId, verticalPrjId;

    std::shared_ptr<Shader> hcs;
    std::shared_ptr<Shader> vcs;
    std::shared_ptr<Shader> shadowShader;
    std::shared_ptr<Shader> lightingShader;
    std::shared_ptr<Shader> locallightsShader;

    glm::mat4 b, shadowMatrix;

    Renderer();
    
};

