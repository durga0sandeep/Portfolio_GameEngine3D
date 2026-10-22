/* *
Copyright(c) 2025 Digipen Institute of Technology
Author : Martin Chow
*/
#include "pch.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderQueue.h"
#include <memory.h>
#include <cstdlib> // for rand()
#include <ctime>   // for time()
#include <UI/UIManager.h>
#include <Graphics/SkeletalAnimation/SkeletonMesh.h>
#include <Physics/PhysicsManager.h>
// #include "EngineInternals.h"
// #include "LightManager.h"
// #include "GameObjectBuilder.h"



std::shared_ptr<Texture> Renderer::getShadowMap() const
{
    return shadowmap;
}

glm::mat4 Renderer::getLightSpaceMatrix() const
{
    return lightSpaceMatrix;
}



/**
 * $BRIEF
 * render function for normal forward rendering objects
 */
void Renderer::normalRender(RenderObjectInfo RenderObject)
{
    auto current_material = RenderObject.getMaterial();

    current_material->setProperty("ModelMatrix", RenderObject.getTransform()->getLocalMatrix());
    current_material->setProperty("ViewMatrix", camera.getView());              // camera.getView());
    current_material->setProperty("ProjectionMatrix", camera.getPerspective()); // camera.getPerspective());
    current_material->setProperty("useTexture", true);
    current_material->setProperty("isTransparent", false);
    current_material->setProperty("viewPos", camera.getEye());
    current_material->setProperty("lightPos", light.getLightPos());
    current_material->setProperty("lightColor", light.lightColor);// glm::vec3(1.0f, 1.0f, 1.0f));
    current_material->setProperty("metallic", 0.0f);
    current_material->setProperty("roughness", 0.5f);
    current_material->setProperty("ambientLight", glm::vec3(0.3f, 0.3f, 0.3f));
    current_material->setProperty("uColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    current_material->setProperty("fadeFactor", 1.0f);
    current_material->setProperty("lightSpaceMatrix", lightSpaceMatrix);
    current_material->setTexture("shadowMap", shadowmap, 1);
    current_material->apply();

    // std::cout << "++++ " << current_material->getTexture("mainTexture")->TestVal << std::endl;
    // current_material->getTexture("mainTexture")->bind(0);
    // std::cout << "ID HERE " << current_material->getTexture("mainTexture")->getID() << std::endl;

    RenderObject.getMesh()->getGeometryBuffer()->bind();

    // renderQueue.opaqueQueue[i].transform->getLocalMatrix()
    if (RenderObject.getMesh()->hasAttribute(GeometryBuffer::Attribute::Position))
    {

        if (RenderObject.getMesh()->getGeometryBuffer()->getIndexCount() > 0)
        {
            glDrawElements(GL_TRIANGLES, RenderObject.getMesh()->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, RenderObject.getMesh()->getGeometryBuffer()->getIndexCount());
        }
    }
    //
    RenderObject.getMesh()->getGeometryBuffer()->unbind();
}

void Renderer::generateShadowMap()
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_WIDTH);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // Set up light's view and projection matrices
    //glm::mat4 lightProjection = glm::ortho(-5000.0f, 5000.0f, -5000.0f, 5000.0f, 0.1f, 1000.0f); // for directional light
    float fovY = glm::radians(60.0f);  // wider FOV covers more scene
    float aspect = SHADOW_WIDTH / float(SHADOW_WIDTH);
    float zNear = 0.1f;
    float zFar = 4028 * 2.0f;
    glm::mat4 lightProjection = glm::perspective(fovY, aspect, zNear, zFar);


    glm::mat4 lightView = glm::lookAt(light.getLightPos(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;

    for (RenderObjectInfo model : renderGraph.getRenderQueue()->getOpaqueQueue())
    {
        if (!model.getMesh()->isGPUReady()) continue;
        if (model.shouldRenderInPass(renderPassState::BASE) || model.shouldRenderInPass(renderPassState::DEFERRED))
        {
            depthMat->setProperty("model", model.getTransform()->getLocalMatrix());
            depthMat->setProperty("lightSpaceMatrix", lightSpaceMatrix);
            depthMat->apply();
            model.getMesh()->getGeometryBuffer()->bind();
            if (model.getMesh()->hasAttribute(GeometryBuffer::Attribute::Position))
            {
                if (model.getMesh()->getGeometryBuffer()->getIndexCount() > 0)
                {
                    glDrawElements(GL_TRIANGLES, model.getMesh()->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
                }
                else
                {
                    glDrawArrays(GL_TRIANGLES, 0, model.getMesh()->getGeometryBuffer()->getIndexCount());
                }
            }
        }



        if (model.shouldRenderInPass(renderPassState::SKELETON))
        {
            //SkeletonRender
            SkeletonShadowRender(model);
        }


    }


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Restore default culling for main scene
    glCullFace(GL_BACK); // Back-face culling is typical for forward rendering
    glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    // Reset viewport to original dimensions
    glViewport(0, 0, width, height);
}

/**
 * $BRIEF
 * render function for normal forward rendering objects
 */
void Renderer::SkeletonRender(RenderObjectInfo RenderObject)
{
    auto current_material = RenderObject.getMaterial();

    SkeletonMesh* skeletonMesh = dynamic_cast<SkeletonMesh*>(RenderObject.getMesh());
    current_material->setProperty("ModelMatrix", RenderObject.getTransform()->getLocalMatrix());
    //current_material->setProperty("NormalTr", glm::inverse(RenderObject.getTransform()->getLocalMatrix()));
    current_material->setProperty("ViewMatrix", camera.getView());              // camera.getView());
    current_material->setProperty("ProjectionMatrix", camera.getPerspective()); // camera.getPerspective());
    current_material->setProperty("InverseNormalMatrix", transpose(inverse(RenderObject.getTransform()->getLocalMatrix())));
    current_material->setProperty("useTexture", current_material->textureIsPresent());
    current_material->setProperty("isTransparent", false);
    current_material->setProperty("viewPos", camera.getEye());
    current_material->setProperty("lightPos", light.getLightPos());
    current_material->setProperty("lightColor", light.lightColor);
    current_material->setProperty("metallic", 0.0f);
    current_material->setProperty("roughness", 0.5f);
    current_material->setProperty("ambientLight", glm::vec3(0.3f, 0.3f, 0.3f));
    current_material->setProperty("uColor", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    current_material->setProperty("fadeFactor", 1.0f);
    current_material->setProperty("lightSpaceMatrix", lightSpaceMatrix);
    current_material->setTexture("shadowMap", shadowmap, 1);
    current_material->apply();
    //skeletonMesh->setObjectTransform();
    skeletonMesh->Draw();
}



void Renderer::SkeletonShadowRender(RenderObjectInfo RenderObject)
{

    // Set up light's view and projection matrices
    glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 1000.0f); // for directional light
    glm::mat4 lightView = glm::lookAt(light.getLightPos(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;

    SkeletonMesh* skeletonMesh = dynamic_cast<SkeletonMesh*>(RenderObject.getMesh());
    depthskeletonMat->setProperty("ModelMatrix", RenderObject.getTransform()->getLocalMatrix());
    depthskeletonMat->setProperty("lightSpaceMatrix", lightSpaceMatrix); // camera.getPerspective());
    //depthskeletonMat->apply();
    //skeletonMesh->setObjectTransform();
    skeletonMesh->DrawShadow(depthskeletonMat);
    //

}



/**
 * $BRIEF
 * render function for skydome
 */
void Renderer::skydomeRender(RenderObjectInfo RenderObject)
{
    if (!RenderObject.getMesh()->isGPUReady()) return;
    glDisable(GL_CULL_FACE);
    auto current_material = RenderObject.getMaterial();

    RenderObject.getTransform()->setPosition(camera.getEye());
    current_material->setProperty("ModelMatrix", RenderObject.getTransform()->getLocalMatrix());
    current_material->setProperty("ViewMatrix", camera.getView());              // camera.getView());
    current_material->setProperty("ProjectionMatrix", camera.getPerspective()); // camera.getPerspective());
    // current_material->setProperty("useTexture", static_cast<GLboolean>(current_material->textureIsPresent()));
    current_material->setProperty("viewPos", camera.getEye()); // or can use inverse of view matrix translation
    current_material->apply();

    RenderObject.getMesh()->getGeometryBuffer()->bind();

    if (RenderObject.getMesh()->hasAttribute(GeometryBuffer::Attribute::Position))
    {

        if (RenderObject.getMesh()->getGeometryBuffer()->getIndexCount() > 0)
        {
            glDrawElements(GL_TRIANGLES, RenderObject.getMesh()->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, RenderObject.getMesh()->getGeometryBuffer()->getIndexCount());
        }
    }

    RenderObject.getMesh()->getGeometryBuffer()->unbind();
    // Restore default culling for main scene
    glCullFace(GL_BACK); // Back-face culling is typical for forward rendering
    glEnable(GL_CULL_FACE);
}

/**
 * $BRIEF
 * constructor for the Renderer class
 */
Renderer::Renderer()
{
}

/**
 * This is a declaration of the singleton for the Renderer Class
 *
 * \return
 */
Renderer& Renderer::instance()
{
    static Renderer instance;
    return instance;
}

void Renderer::clear(float r, float g, float b, float a)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(r, g, b, a);
}

void Renderer::drawUI(unsigned int dataCount, GLenum drawingMode, bool indexed)
{

}

void Renderer::initialize(GameWindow* window)
{
    glfwMakeContextCurrent(window->getNativeWindow());

    // Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        assert("Failed to initialize GLAD");
        throw std::runtime_error("Failed to initialize GLAD");
    }
    //Multi samples
    glEnable(GL_MULTISAMPLE);
    // Optional: Enable depth testing
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_DEBUG_OUTPUT);

    height = window->getHeight();
    width = window->getWidth();

    particlesys = &ParticleSystemManager::instance();
    PhysicsManager::instance().setupdebugRender();
}



void Renderer::draw(unsigned int dataCount, GLenum drawingMode, bool indexed)
{

    if (indexed)
    {
        glDrawElements(drawingMode, dataCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(drawingMode, 0, dataCount);
    }
}

void Renderer::shutdown()
{
}

void Renderer::run()
{
    /*EngineInternals::GetWindow().swapBuffers();
    render(EngineInternals::GetSceneManager().getSceneGraph());*/
}

void Renderer::render(GLFWwindow* window)
{
    glfwPollEvents();
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    deferredRendering();

    // CRITICAL FIX: Copy depth buffer from G-buffer to default framebuffer
    // Replace gBufferFBO and screenWidth/screenHeight with your actual values
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);  // Your G-buffer FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);           // Default framebuffer
    glBlitFramebuffer(
        0, 0, width, height,
        0, 0, width, height,
        GL_DEPTH_BUFFER_BIT,
        GL_NEAREST
    );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto opaqueQueue = renderGraph.getRenderQueue()->getOpaqueQueue();


    // Skydome rendering (forward pass)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    if (debug)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // 1. SKYDOME (depth write off)
    glDepthMask(GL_FALSE);

    for (int i = 0; i < opaqueQueue.size(); i++)
    {
        if (opaqueQueue[i].shouldRenderInPass(renderPassState::SKYDOME))
        {
            if (opaqueQueue[i].getMesh()->isGPUReady())
                skydomeRender(opaqueQueue[i]);
        }
    }
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ParticleSystemManager::instance().render(camera.getView(), camera.getPerspective());
    glDisable(GL_BLEND);

    //for (int i = 0; i < opaqueQueue.size(); i++)
    //{
    //    /*if (opaqueQueue[i].shouldRenderInPass(renderPassState::BASE))
    //    {
    //        if (opaqueQueue[i].getMesh()->isGPUReady())
    //            normalRender(opaqueQueue[i]);
    //    }*/
    //   /* if (opaqueQueue[i].shouldRenderInPass(renderPassState::SKELETON))
    //    {
    //        if (opaqueQueue[i].getMesh()->isGPUReady())
    //            SkeletonRender(opaqueQueue[i]);
    //    }*/
    //}



    if (debug)
    {
        PhysicsManager::instance().debugRender();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    

    // 4. UI (always on top)
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    UIManager::instance().render();
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    //glEnable(GL_DEPTH_TEST);

    glfwSwapBuffers(window);
    camera.update();
}

void Renderer::initializeRenderObjects()
{
    sphereMesh1 = AssetLoadManager::instance().setAssetMesh(AssetLoadManager::instance().sphere_path);

    ssbo.setupSSBO();
    ssboSpotLight.setupSSBO();

    //creatingPointLights(glm::vec3(0, 10, 0), glm::vec3(1.0, 0, 0),20,5);
    // Camera
    Camera& cam = camera;
    cam.initializeCam();
    cam.update();

    light.update();
    setupShadowMap();


    distLightToOrigin = glm::length(light.getLightPos());
    zNear = 0.1f; // distLightToOrigin - 40;
    zFar = 1000.0f; //distLightToOrigin + 40;


}





void Renderer::deferredRendering()
{

    glEnable(GL_DEPTH_TEST);
    // 1.1 Shadow Pass: renders shadow
    //shadowPass();
    generateShadowMap();

    glEnable(GL_DEPTH_TEST);
    // 1. Geometry Pass: render scene's geometry/color data to G-buffer
    geometryPass();

    glDisable(GL_DEPTH_TEST);
    // 2. Lighting Pass: use G-buffer to calculate lighting
    lightingPass();
    
    manyLocalLightsPass( );

    manySpotLightsPass();
}

/**
 * @brief Initializes all resources required for deferred rendering.
 *
 * Creates G-buffer textures, framebuffers, shaders, materials,
 * fullscreen quad, sphere mesh for local lights, shadow maps, and
 * other necessary buffers for deferred rendering pipeline.
 */
void Renderer::initDeferredRendering()
{
    //
    //This is for the setup of the shadow
    //setUpShadow();

    msmMethods.createUniformBuffer();

    hcs = make_shared<Shader>(AssetLoadManager::instance().shader_path_Hcs);

    glGenTextures(1, &hcs->BlurSMTextureId);
    glBindTexture(GL_TEXTURE_2D, hcs->BlurSMTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);


    vcs = make_shared<Shader>(AssetLoadManager::instance().shader_path_Vcs);

    glGenTextures(1, &vcs->FinalSMTextureId);
    glBindTexture(GL_TEXTURE_2D, vcs->FinalSMTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::shared_ptr<Shader> shadowShader = make_shared<Shader>(AssetLoadManager::instance().shader_path_Shadow);
    shadowMaterial = std::make_shared<Material>(shadowShader);

    //
    //local light shader
    std::shared_ptr<Shader> localLightShader = make_shared<Shader>(AssetLoadManager::instance().LocalLightsShader);//
    pointlightMaterial = std::make_shared<Material>(localLightShader);
    std::shared_ptr<Shader> spotLightShader = make_shared<Shader>(AssetLoadManager::instance().SpotLightsShader);//
    spotlightMaterial = std::make_shared<Material>(localLightShader);
    //
    //--- Initialize shaders
    //std::shared_ptr<Shader> lightingShader = make_shared<Shader>(assetMgr.shader_path_DeferredLight);
    std::shared_ptr<Shader> lightingShader = make_shared<Shader>(AssetLoadManager::instance().DeferredLight);

    fullscreenQuad = std::make_shared<Quad>("quad");
    lightingMaterial = std::make_shared<Material>(lightingShader);
    sphereMesh = std::make_shared<SphereClass>("sphereHuge", 32, 1.0f);



    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    //const int width = screenWidth;
    //const int height = screenHeight;
  

    struct TextureSpec {
        GLenum internalFormat;
        GLenum format;
        GLenum type;
        int channels;
    };

    // All outputs are vec4 → use RGBA16F for high precision
    std::vector<TextureSpec> specs{ {
        { GL_RGBA16F, GL_RGBA, GL_FLOAT, 4 }, // Position (vec4)
        { GL_RGBA16F, GL_RGBA, GL_FLOAT, 4 }, // Normal (vec4)
        { GL_RGBA16F, GL_RGBA, GL_FLOAT, 4 }, // Diffuse (vec4)
        { GL_RGBA16F, GL_RGBA, GL_FLOAT, 4 }  // Specular + Alpha (vec4)
    } };


    gbufferTextures.resize(specs.size());


    for (size_t i = 0; i < specs.size(); ++i)
    {
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, specs[i].internalFormat, width, height, 0, specs[i].format, specs[i].type, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texID, 0);

        gbufferTextures[i] = std::make_shared<Texture>(texID, width, height, specs[i].channels, specs[i].format, Texture::Type::Texture2D);
    }

    // Set draw buffers
    std::vector<GLenum> attachments(specs.size());
    for (size_t i = 0; i < attachments.size(); ++i)
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data());

    // Create depth buffer
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    // Check completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


   

    //
    //Creating Point Lights
    

}



/**
 * @brief Returns a scaling matrix.
 *
 * Constructs a 4x4 scale matrix using the provided x, y, z scaling factors.
 *
 * @param x Scale factor along the X-axis.
 * @param y Scale factor along the Y-axis.
 * @param z Scale factor along the Z-axis.
 * @return glm::mat4 Scaling matrix.
 */
glm::mat4 Renderer::scale(const float x, const float y, const float z)
{
    glm::mat4 S = glm::mat4(1.0f);
    S[0][0] = x;
    S[1][1] = y;
    S[2][2] = z;
    return S;
}




/**
 * @brief Returns a translation matrix.
 *
 * Constructs a 4x4 translation matrix using the provided x, y, z offsets.
 *
 * @param x Translation along the X-axis.
 * @param y Translation along the Y-axis.
 * @param z Translation along the Z-axis.
 * @return glm::mat4 Translation matrix.
 */
glm::mat4 Renderer::translate(const float x, const float y, const float z)
{
    glm::mat4 T = glm::mat4(1.0);
    T[3][0] = x;
    T[3][1] = y;
    T[3][2] = z;

    return T;
}


/**
 * @brief Creates a grid of point lights and uploads them to a Shader Storage Buffer Object (SSBO).
 *
 * This method initializes multiple point lights in a 5x5 grid pattern with fixed
 * spacing and height. Each light is assigned a color, intensity, and radius.
 * After creating all lights, the method uploads the point light data to the GPU
 * using the SSBO.
 *
 * The grid layout is as follows:
 * - X-axis moves right across columns
 * - Z-axis moves forward across rows
 * - Y-axis is fixed height for all lights
 *
 * @note Currently, all lights are red with intensity 3.0 and radius 5.0.
 *       You can modify color, intensity, and radius per light if needed.
 */
int Renderer::creatingPointLights(glm::vec3 pos, glm::vec3 color, float intensity, float radius , const std::string& meshPath)
{
    auto mesh = AssetLoadManager::instance().setAssetMesh(meshPath.c_str());
    return pointLightScript.CreateLights(pos, color, intensity, radius, mesh);
}

int Renderer::creatingSpotLights(glm::vec3 pos, glm::vec3 color, float intensity, float radius, const std::string& meshPath)
{
    auto mesh = AssetLoadManager::instance().setAssetMesh(meshPath.c_str());
    return spotLightScript.CreateLights(pos, color, intensity, radius, mesh);
}


void Renderer::setupShadowMap()
{
    glGenFramebuffers(1, &shadowFBO);

    // Create depth texture
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, // or GL_DEPTH_COMPONENT32F for more precision
        SHADOW_WIDTH, SHADOW_WIDTH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // LINEAR for PCF
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Enable depth comparison for sampler2DShadow
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    // Set border color (1.0 = lit, 0.0 = shadowed for areas outside shadow map)
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    // No color buffer is drawn to
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Check framebuffer completeness
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Error: Shadow framebuffer is not complete! Status: 0x"
            << std::hex << status << std::dec << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Store texture in your wrapper
    shadowmap = std::make_shared<Texture>(depthTexture, SHADOW_WIDTH, SHADOW_WIDTH,
        1, GL_FLOAT, Texture::Type::Texture2D);

    // Create depth shader and material
    std::shared_ptr<Shader> depthShader = std::make_shared<Shader>(
        AssetLoadManager::instance().shader_path_depth);
    depthMat = std::make_shared<Material>(depthShader);


    // Create depth shader and material
    std::shared_ptr<Shader> skeletalShadowShader = std::make_shared<Shader>(
        AssetLoadManager::instance().shader_skeleton_depth);
    depthskeletonMat = std::make_shared<Material>(skeletalShadowShader);

}



RenderObjectInfo& Renderer::initializeObjects(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, Transform* trans)
{

    return renderGraph.createRenderObjects(mesh, mat, trans);
}

RenderObjectInfo& Renderer::initializeObjects(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, Transform* trans, renderPassState state)
{
    return renderGraph.createRenderObjects(mesh, mat, trans, state);
}

RenderGraph& Renderer::getRenderGraph()
{
    return renderGraph;
}



/**
 * @brief Performs the geometry pass for deferred rendering.
 *
 * This pass writes scene geometry and material properties to the G-buffer.
 * Each object in the opaque queue that should be rendered in the deferred pass
 * has its model and normal matrices set in the material shader, then drawn.
 * 
 **/
void Renderer::geometryPass()
{
    // 1. Bind the G-buffer framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. Use the geometry shader
    // 3. Set view/projection matrices (assumes you have a camera object)
    glm::mat4 projection = camera.getPerspective();
    glm::mat4 view = camera.getView();

    // 4. Render all models in the scene
    for (RenderObjectInfo model : renderGraph.getRenderQueue()->getOpaqueQueue()) {
        if (model.getIsVisible() == false) continue;
        if (model.getMesh()->isGPUReady()) {

            if (model.shouldRenderInPass(renderPassState::DEFERRED)) {
                Material* gbufferMaterial = model.getMaterial();
                glm::mat4 modelMatrix = model.getTransform()->getLocalMatrix();
                gbufferMaterial->setProperty("ModelTr", modelMatrix);
                glm::mat4 NormalTr = glm::transpose(glm::inverse(modelMatrix));
                gbufferMaterial->setProperty("NormalTr", NormalTr);
                gbufferMaterial->setProperty("WorldProj", projection);
                gbufferMaterial->setProperty("WorldView", view);
                //TODO: remove this and force materials to have their own specular property
                gbufferMaterial->setProperty("specular", glm::vec3(0.3));
                gbufferMaterial->setProperty("shininess", 16.0f);

                // Apply material (binds shader and sets uniforms)
                // this also apply texture
                gbufferMaterial->apply();

                // Bind VAO and draw
                model.getMesh()->getGeometryBuffer()->bind();
                glDrawElements(GL_TRIANGLES, model.getMesh()->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);

                // Reset texture bindings
                glActiveTexture(GL_TEXTURE0);
            }
            if (model.shouldRenderInPass(renderPassState::SKELETON)) {
                cout << "model active:" << model.getIsVisible() << endl;
                Material* gbufferMaterial = model.getMaterial();
                gbufferMaterial->setProperty("ModelMatrix", model.getTransform()->getLocalMatrix());
                gbufferMaterial->setProperty("ViewMatrix", camera.getView());              // camera.getView());
                gbufferMaterial->setProperty("ProjectionMatrix", camera.getPerspective()); // camera.getPerspective());
                gbufferMaterial->setProperty("InverseNormalMatrix", transpose(inverse(model.getTransform()->getLocalMatrix())));
                gbufferMaterial->apply();
                SkeletonMesh* skeletonMesh = dynamic_cast<SkeletonMesh*>(model.getMesh());
                skeletonMesh->Draw();
            }
        }


    }

    // 5. Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}



/**
 * @brief Sets up the framebuffer and texture for shadow mapping.
 *
 * This function creates the shadow FBO, attaches a depth renderbuffer,
 * creates a shadow texture, sets wrapping/filtering, and binds it to the FBO.
 */
//void Renderer::setUpShadow() {
//
//    //
//    //Creating the Fbo to be used for the shadows
//    //Setting SHADOW FBO
//    //
//    //Create FBO ---> Frame Buffer Object 
//    glGenFramebuffers(1, &shadowFboId);
//    glBindFramebuffer(GL_FRAMEBUFFER, shadowFboId);
//
//    glGenRenderbuffers(1, &depthBuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_WIDTH);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
//
//    //
//    //Setting texture 
//    glGenTextures(1, &shadowTextureID);
//    glBindTexture(GL_TEXTURE_2D, shadowTextureID);
//    glTexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA32F, SHADOW_WIDTH, SHADOW_WIDTH, 0, GL_RGBA, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR);
//
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowTextureID, 0);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    //
//}


/**
 * @brief Renders the scene from the light's perspective into the shadow map.
 *
 * Performs the shadow pass using the shadow FBO. Sets light view/projection,
 * binds the shadow material, and draws each object that participates in deferred rendering.
 */

//void Renderer::shadowPass()
//{
//
//    glBindFramebuffer(GL_FRAMEBUFFER, shadowFboId);//
//
//    glViewport(0, 0, SHADOW_WIDTH, SHADOW_WIDTH);
//    glClearColor(0.5, 0.5, 0.5, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glm::vec3 center(0.0f, 0.0f, 0.0f); // center of your scene
//
//    lightProjection = glm::ortho(
//        -halfSize , halfSize ,
//        -halfSize , halfSize ,
//        zNear, zFar
//    );
//
//
//    lightView = glm::lookAt(light.getLightPos(), center, glm::vec3(0.0f, 1.0f, 0.0f));
//    //glm::mat4 lightView = glm::lookAt(camera.getEye(), center, glm::vec3(0.0f, 1.0f, 0.0f));
//
//    lightSpaceMatrix = lightProjection * lightView;
//
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_FRONT);
//
//    //
//    // Render the objects to the shadow map
//    for (RenderObjectInfo model : renderGraph.getRenderQueue()->getOpaqueQueue()) {
//        if (model.shouldRenderInPass(renderPassState::DEFERRED)) {
//            shadowMaterial->setProperty("model", model.getTransform()->getLocalMatrix());
//            shadowMaterial->setProperty("lightSpaceMatrix", lightSpaceMatrix);
//            shadowMaterial->setProperty("z0", zNear);
//            shadowMaterial->setProperty("z1", zFar);
//            shadowMaterial->apply();
//            model.getMesh()->getGeometryBuffer()->bind();
//
//            // Check for mesh attributes (assumed to exist)
//            if (model.getMesh()->hasAttribute(GeometryBuffer::Attribute::Position)) {
//                if (model.getMesh()->getGeometryBuffer()->getIndexCount() > 0) {
//                    glDrawElements(GL_TRIANGLES, model.getMesh()->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
//                }
//                else {
//                    glDrawArrays(GL_TRIANGLES, 0, model.getMesh()->getGeometryBuffer()->getIndexCount());
//                }
//            }
//        }
//
//        /*
//        if(model.shouldRenderInPass(renderPassState::SKELETON))
//        {
//            SkeletonShadowRender(model);
//        }
//        */
//    }
//
//    // Restore default culling for the main scene
//    glDisable(GL_CULL_FACE);
//
//    //shadowMaterial->getShader()->unuse();
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);//
//
//    glViewport(0, 0, width, height);
//
//}



/**
 * @brief Performs horizontal and vertical compute shader passes for shadow map blurring.
 *
 * Uses compute shaders to blur the shadow map: first horizontally, then vertically.
 * Sets image bindings, uniform block bindings, and dispatches the compute shaders.
 */
void Renderer::horizVerticalCS()
{
    hcs->use();
    horzPrjId = hcs->getID();

    //Sending or binding the shadowMap to the Horizontal Compute Shader
    glBindImageTexture(0, shadowTextureID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    int loc = glGetUniformLocation(horzPrjId, "shadowMap");
    glUniform1i(loc, 0);

    //Fbo->BindImageTexture_CS(1, GL_WRITE_ONLY, GL_RGBA32F, HorzPrjId, "blurredShadowMap");//
    hcs->BindImageTexture(1, hcs->BlurSMTextureId, GL_WRITE_ONLY, horzPrjId, "blurredShadowMap");

    loc = glGetUniformBlockIndex(horzPrjId, "blurKernal");
    glUniformBlockBinding(horzPrjId, loc, msmMethods.bindpoint);

    GLint uniformLocation1 = glGetUniformLocation(horzPrjId, "blurWidth");
    glUniform1i(uniformLocation1, msmMethods.blurWidth);


    int groupsX = (SHADOW_WIDTH + 127) / 128; // = 4096/128 = 32
    int groupsY = SHADOW_WIDTH;

    //hcs->DispatchComputerShader(1024, 1024, 128, 1);
    hcs->DispatchComputerShader(32, 4096, 0, 1);
    //hcs->DispatchComputerShader(SHADOW_WIDTH, SHADOW_WIDTH, 128, 1);

    vcs->use();
    verticalPrjId = vcs->getID();

    vcs->BindImageTexture(0, hcs->BlurSMTextureId, GL_READ_ONLY, verticalPrjId, "blurredShadowMap");
    vcs->BindImageTexture(1, vcs->FinalSMTextureId, GL_WRITE_ONLY, verticalPrjId, "finalShadowMap");

    loc = glGetUniformBlockIndex(verticalPrjId, "blurKernal");
    glUniformBlockBinding(verticalPrjId, loc, msmMethods.bindpoint);

    GLint uniformLocation = glGetUniformLocation(verticalPrjId, "blurWidth");
    glUniform1i(uniformLocation, msmMethods.blurWidth);

    //vcs->DispatchComputerShader(1024, 1024, 1, 128);
    vcs->DispatchComputerShader(4096, 32, 0, 1);
    //vcs->DispatchComputerShader(SHADOW_WIDTH, SHADOW_WIDTH, 1, 128);

}

/**
 * @brief Performs the lighting pass in deferred rendering.
 *
 * Combines information from the G-buffer and the shadow map to compute lighting
 * for the scene. Binds all relevant textures (position, normal, albedo, material),
 * sets uniforms (light color, ambient, camera transforms), computes shadow matrix,
 * and renders a fullscreen quad to apply lighting.
 */
void Renderer::lightingPass()
{
    glDisable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 WorldInverse = glm::inverse(camera.getView());
    //
    b = translate(0.5, 0.5, 0.5) * scale(0.5, 0.5, 0.5);
    glm::mat4 lightView = light.getLightViewMatrix();
    glm::mat4 lightProjection = light.getLightOrthoProjection(newFrustumSize, nearPlane, farPlane);
    shadowMatrix = b * lightProjection * lightView;
    //

    // 4. Set lighting uniforms
    lightingMaterial->setTexture("gPosition", gbufferTextures[0], 0);
    lightingMaterial->setTexture("gNormal", gbufferTextures[1], 1);
    lightingMaterial->setTexture("gAlbedo", gbufferTextures[2], 2);
    lightingMaterial->setTexture("shadowMap", shadowmap, 3);

    lightingMaterial->setProperty("lightSpaceMatrix", lightSpaceMatrix);
    lightingMaterial->setProperty("lightPos", light.getLightPos());
    lightingMaterial->setProperty("lightColor", light.lightColor);// glm::vec3(1.0f, 1.0f, 1.0f));
    lightingMaterial->setProperty("ambientLight", glm::vec3(0.3f, 0.3f, 0.3f));
    lightingMaterial->setProperty("viewPos", camera.getEye());
    lightingMaterial->apply();

    // 5. Render fullscreen quad
    fullscreenQuad->getGeometryBuffer()->bind(); // assumes you have a fullscreen quad VAO
    glDrawElements(GL_TRIANGLES, fullscreenQuad->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 6. Reset texture bindings
    glActiveTexture(GL_TEXTURE0);
}



/**
 * @brief Renders multiple local (point) lights in the scene.
 *
 * Iterates over all local lights, binds G-buffer textures, sets uniforms
 * (view, projection, world inverse), and draws a sphere for each light
 * to accumulate its contribution in the deferred lighting pipeline.
 */
void Renderer::manyLocalLightsPass()
{
    if (pointLightScript.GetPointLights().empty())return;
    ssbo.updateSSBO(pointLightScript.GetPointLights(), pointLightScript.structSize());

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);


    glm::mat4 projection = camera.getPerspective();
    glm::mat4 view = camera.getView();

 
    pointlightMaterial->setProperty("WorldProj", projection);
    pointlightMaterial->setProperty("WorldView", view);

    pointlightMaterial->setProperty("cameraPos", camera.getEye()); 
    pointlightMaterial->setProperty("screenSize", glm::vec2(width, height));

    pointlightMaterial->setTexture("gPosition", gbufferTextures[0], 0);
    pointlightMaterial->setTexture("gNormal", gbufferTextures[1], 1);
    pointlightMaterial->setTexture("gAlbedo", gbufferTextures[2], 2);

    localLightsPrgId = pointlightMaterial->getShader()->getID();



    for (int i = 0; i < pointLightScript.listDataSize(); i++) {

        auto LightData = pointLightScript.GetPointLight(i);
        //LightData.color = glm::vec3(1.0f, 0.0f, 0.0f); // light blue color

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, LightData.position);
        //model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, pointLightScript.GetScale(i));
        //model = glm::scale(model, glm::vec3(100));

        pointlightMaterial->setProperty("model", model);

        /*
         1.0 - Normal intensity
         2.0 - Twice as bright
         0.5 - Half brightness
         5.0+ - Very bright, useful for strong light sources like the sun
        */
        pointlightMaterial->setProperty("lightPos", LightData.position);
        pointlightMaterial->setProperty("lightColor", LightData.color);
        pointlightMaterial->setProperty("lightIntensity", 5.0f);
        pointlightMaterial->setProperty("lightRadius", 3.0f);

        pointlightMaterial->apply();
        auto pointLightMesh = pointLightScript.getMesh(i);
        if(pointLightMesh->isGPUReady()) {
            pointLightMesh->getGeometryBuffer()->bind();
            //sphereMesh->getGeometryBuffer()->bind();
            glDrawElements(GL_TRIANGLES, pointLightMesh->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
            //glDrawElements(GL_TRIANGLES, SphereRenderObj.getMesh()->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    glDisable(GL_BLEND);
}

/**
 * @brief Renders multiple local (point) lights in the scene.
 *
 * Iterates over all local lights, binds G-buffer textures, sets uniforms
 * (view, projection, world inverse), and draws a sphere for each light
 * to accumulate its contribution in the deferred lighting pipeline.
 */
void Renderer::manySpotLightsPass()
{
    if (spotLightScript.GetPointLights().empty())return;
    ssboSpotLight.updateSSBO(spotLightScript.GetPointLights(), spotLightScript.structSize());

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);


    glm::mat4 projection = camera.getPerspective();
    glm::mat4 view = camera.getView();


    spotlightMaterial->setProperty("WorldProj", projection);
    spotlightMaterial->setProperty("WorldView", view);

    spotlightMaterial->setProperty("cameraPos", camera.getEye());
    spotlightMaterial->setProperty("screenSize", glm::vec2(width, height));

    spotlightMaterial->setTexture("gPosition", gbufferTextures[0], 0);
    spotlightMaterial->setTexture("gNormal", gbufferTextures[1], 1);
    spotlightMaterial->setTexture("gAlbedo", gbufferTextures[2], 2);
    const glm::vec3 spotDirection = glm::vec3(0, -1, 0); // Always points downward
    const float spotInnerCone = 0.906; // cos(25°)
    const float spotOuterCone = 0.819; // cos(35°)

    spotlightMaterial->setProperty("spotDirection", spotDirection);
    spotlightMaterial->setProperty("spotInnerCone", spotInnerCone);
    spotlightMaterial->setProperty("spotOuterCone", spotOuterCone);
    localLightsPrgId = spotlightMaterial->getShader()->getID();



    for (int i = 0; i < spotLightScript.listDataSize(); i++) {

        auto LightData = spotLightScript.GetPointLight(i);
        //LightData.color = glm::vec3(1.0f, 0.0f, 0.0f); // light blue color

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, LightData.position);
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, spotLightScript.GetScale(i));
        //model = glm::scale(model, glm::vec3(100));

        spotlightMaterial->setProperty("model", model);

        /*
         1.0 - Normal intensity
         2.0 - Twice as bright
         0.5 - Half brightness
         5.0+ - Very bright, useful for strong light sources like the sun
        */
        spotlightMaterial->setProperty("lightPos", LightData.position);
        spotlightMaterial->setProperty("lightColor", LightData.color);
        spotlightMaterial->setProperty("lightIntensity", 5.0f);
        spotlightMaterial->setProperty("lightRadius", 3.0f);

        spotlightMaterial->apply();
        auto pointLightMesh = spotLightScript.getMesh(i);
        if (pointLightMesh->isGPUReady()) {
            pointLightMesh->getGeometryBuffer()->bind();
            //sphereMesh->getGeometryBuffer()->bind();
            glDrawElements(GL_TRIANGLES, pointLightMesh->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
            //glDrawElements(GL_TRIANGLES, SphereRenderObj.getMesh()->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    glDisable(GL_BLEND);
}



/**
 * @brief Placeholder for post-processing pass.
 *
 * This method is currently empty. Intended for future post-processing
 * effects like bloom, tone mapping, or color grading after lighting passes.
 */
void Renderer::postProcessingPass()
{



}
