/*!****************************************************************************
 * file  : AssetLoadManager.h
 * brief : AssetLoadManager - this script is used to load the assets using the
 * - Assimp library for 3D art assets.
 * - We will be loading the Audio assets.
 * - Images for the textures. *
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include "Graphics/Mesh.h"
#include "Graphics/Texture.h"
#include "Memory/ResourceManager.h"
#include <stdlib.h>
#include <future>
#include <queue>
#include "LuaManager/LuaManager.h"
#include "JobSystem/MyJobSystem.h"

/**
 * @class AssetLoadManager
 * @brief Singleton manager for loading and processing game assets such as meshes, textures, and shaders.
 *
 * The AssetLoadManager provides a centralized interface to load models (OBJ, FBX),
 * textures, and shader files, using Assimp for mesh processing and standard file I/O
 * for shader source code. It ensures proper memory management using shared pointers
 * and follows the singleton pattern to maintain a single instance across the engine.
 */
class AssetLoadManager
{

public:
    /**
     * @brief Default model file path for testing asset loading.
     */
    const char *file_path = "Model/Obj/box.obj";//Model/Fbx/char_a.fbx
    const char* box_path = "Model/Obj/box.obj";//Model/Fbx/char_a.fbx
    const char *Char_path = "Model/Fbx/char_a.fbx";///blaster-a.fbx";
    const char *Car_path = "Model/Fbx/blaster-a.fbx";
    const char* texture_path = "Texture/char_a.png";
    const char* sphere_path = "Model/Fbx/sphere.fbx";

    /**
     * @brief Paths to vertex and fragment shader files (deferred shading).
     *
     * Vertex shader: Shader/vertex.vs
     * Fragment shader: Shader/fragment.fs
     */
    const char *shader_path = "Shader/vertex.vs\nShader/fragment.fs";
    

    /**
     * @brief Path to the G-Buffer vertex shader file.
     */
    //const char *shader_path_GBuffer = "Shader/GBuffer_vertex.vs\nShader/GBuffer_fragment.fs";

    /**
     * @brief Path to the G-Buffer fragment shader file.
     */
    const char *shader_path_Skydome = "Shader/sky_vertex.glsl\nShader/sky_fragment.glsl";
    const char* shader_path_Skeleton = "Shader/skeleton_vertex.glsl\nShader/skeleton_fragment.glsl";
    const char *shader_path_PhongLight = "Shader/lightingPhong_vertex.vs\nShader/lightingPhong_fragment.fs";
    const char* shader_path_depth = "Shader/shadow_vertex_shader.glsl\nShader/shadow_fragment_shader.glsl";
    const char* shader_skeleton_depth = "Shader/skeleton_shadow_vertex.glsl\nShader/skeleton_shadow_fragment.glsl";
    //
    //Deferred 
    //
    const char* shader_path_GBuffer = "Shader/GBuffer_vertex.glsl\nShader/GBuffer_fragment.glsl";
    const char* shader_path_skeleton_GBuffer = "Shader/gBuffer_skeleton_vertex.vs\nShader/gBuffer_skeleton_fragment.fs";
    const char* shader_path_Shadow = "Shader/shadow_vertex.glsl\nShader/shadow_fragment.glsl";
    //const char* shader_path_DeferredLight = "Shader/deffer_lightingPhong_vertex.glsl\nShader/deffer_lightingPhong_fragment.glsl";
    const char* shader_path_Hcs = "Shader/test_defermsm/Horiz_compute.comp";
    const char* shader_path_Vcs = "Shader/test_defermsm/Vertical_compute.comp";
    const char* DeferredLight = "Shader/test_defermsm/lighting_vertex.vert\nShader/test_defermsm/defferredversion1_fragment.glsl";
    //const char* LocalLightsShader = "Shader/locallight_vertex.vert\nShader/locallight_fragment.frag";
    const char* LocalLightsShader = "Shader/test_defermsm/s_vertex.glsl\nShader/test_defermsm/s_fragment.glsl";
    const char* SpotLightsShader = "Shader/spotlight_vertex.glsl\nShader/spotlight_fragment.glsl";

    // 


    //
    //
    //


    //
    AssetLoadManager();
    ~AssetLoadManager();
    //

    /**
     * @brief Provides access to the singleton instance of AssetLoadManager.
     *
     * This method ensures that only one instance of AssetLoadManager exists
     * throughout the program. The instance is created on the first call and
     * reused for subsequent calls.
     *
     * @return Reference to the singleton AssetLoadManager instance.
     */
    static AssetLoadManager & instance()
    {
        static AssetLoadManager instance;
        return instance;
    }

    /*
    //these are old in  my branch - sandeep --- remove if not needed
    std::shared_ptr<Mesh> assetMesh(const char * input_file_path);
    std::shared_ptr<Mesh> assetMeshSecond(const char * input_file_path);
    std::shared_ptr<Mesh> processMesh(const aiMesh *node);
    */
    std::shared_ptr<Mesh> processMeshSecond(const aiMesh *node);

    std::shared_ptr<Mesh> setAssetMesh(const char *Meshpath);
    //std::shared_ptr<Mesh> processMesh(const aiMesh *node);

    std::shared_ptr<Texture> assetTexture(const char *Texturepath);

    std::string readShaderFile(const std::string &filePath);

    std::shared_ptr<Texture> loadTextureImage(const std::string& filename);

    //static void waitForLoads();
    //void waitForAllPendingLoads();
    void incrementPendingLoad() { pending_loads++; }
    void decrementPendingLoad() { pending_loads--; }
    bool areAllLoadsComplete() const;
    void processGPUUploads(); // Call this on main thread each frame
    static void bindToLua(sol::state& lua);
    //void loadAssetsTest();

    //void loadAudio();

private:
    std::atomic<int> pending_loads{ 0 };
    std::pair<std::vector<float>, std::vector<unsigned int>>
        processMeshData(const aiMesh* mesh);

    void loadMeshDataAsync(std::shared_ptr<Mesh> meshPtr, const std::string& path);

    struct GPUUploadTask {
        std::shared_ptr<Mesh> mesh;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    };
    mutable std::mutex futuresMutex_;
    std::vector<std::future<void>> pendingLoads_;

    // GPU upload queue for main thread
    mutable std::mutex uploadMutex_;
    std::queue<GPUUploadTask> uploadQueue_;

    MyJobSystem jobSystem_;

    // External loading state tracking
    mutable std::mutex stateMutex_;
    std::unordered_map<std::string, bool> meshLoadStatus_;
    std::unordered_map<std::string, std::string> meshLoadErrors_;
};
