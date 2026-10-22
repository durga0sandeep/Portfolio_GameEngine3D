/*!****************************************************************************
 * file  : AssetLoadManager.cpp
 * brief : Implements the AssetLoadManager class responsible for loading,
 *         game assets such as textures, models, and audio files.
 *         Provides centralized control for asset laoding.
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 *****************************************************************************/

#include "pch.h"
#include <memory>
#include "Memory/AssetLoadManager.h"
#include <Graphics/Texture.h>


/**
 * $BRIEF
 * asset Mesh with async loading on Mesh
 * IMportant!!!!!!
 * 1. please do not call setVertexData and assetMesh on other thread
 * 2. waitForAllPendingLoads when you done loading
 * \param input_file_path
 * \return \b 
 */
std::shared_ptr<Mesh> AssetLoadManager::setAssetMesh(const char* input_file_path)
{
    // check if exists
    if (auto existing = ResourceManager<Mesh>::retrieve(input_file_path)) {
        return existing;
    }

    // Create placeholder Mesh and register it
    auto meshPtr = std::make_shared<Mesh>(input_file_path);
    ResourceManager<Mesh>::create(input_file_path, meshPtr);

    // Mark as loading in our external state tracker
    {
        std::lock_guard<std::mutex> lock(stateMutex_);
        meshLoadStatus_[input_file_path] = false; // false = loading, true = loaded
    }

    // Submit async load job to our job system
    jobSystem_.Submit([this, meshPtr, path = std::string(input_file_path)]() {
        loadMeshDataAsync(meshPtr, path);
        });

    return meshPtr;
}



/**
 * $BRIEF
 * the Async function to use assimp to import the mesh and save the vertex data to a vector
 * \param meshPtr
 * \param path
 */
void AssetLoadManager::loadMeshDataAsync(std::shared_ptr<Mesh> meshPtr, const std::string& path)
{
    try {
        Assimp::Importer importer;
        auto scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_JoinIdenticalVertices |
            aiProcess_ValidateDataStructure |
            aiProcess_ImproveCacheLocality |
            aiProcess_SortByPType);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            {
                std::lock_guard<std::mutex> lock(stateMutex_);
                meshLoadErrors_[path] = importer.GetErrorString();
            }
            return;
        }

        if (scene->mNumMeshes == 0) {
            std::cerr << "No meshes found in file: " << path << std::endl;
            {
                std::lock_guard<std::mutex> lock(stateMutex_);
                meshLoadErrors_[path] = "No meshes found in file";
            }
            return;
        }

        // Extract CPU-side data
        auto mesh = scene->mMeshes[0];
        auto [vertices, indices] = processMeshData(mesh);

        // Queue GPU upload for main thread instead of doing it here
        {
            std::lock_guard<std::mutex> lock(uploadMutex_);
            uploadQueue_.push({ meshPtr, std::move(vertices), std::move(indices) });
        }

        // Mark as successfully loaded (CPU data is ready, GPU upload queued)
        {
            std::lock_guard<std::mutex> lock(stateMutex_);
            meshLoadStatus_[path] = true;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Exception during mesh loading: " << e.what() << std::endl;
        {
            std::lock_guard<std::mutex> lock(stateMutex_);
            meshLoadErrors_[path] = e.what();
        }
    }
    std::cout << "Finished async loading for mesh: " << path << std::endl;
}

/**
 * $BRIEF
 * load all stuff to GPU
 * IMPORTANT!!!! must call this from main thread because opengl is dump
 */
void AssetLoadManager::processGPUUploads()
{
    std::queue<GPUUploadTask> localQueue;

    // Quickly swap out the queue to minimize lock time
    {
        std::lock_guard<std::mutex> lock(uploadMutex_);
        std::swap(localQueue, uploadQueue_);
    }

    // Process all GPU uploads on the main thread
    while (!localQueue.empty()) {
        auto& task = localQueue.front();

        // This now happens on the main thread where it's safe
        task.mesh->setVertexData({
            {GeometryBuffer::Attribute::Position, task.vertices, 3},
            {GeometryBuffer::Attribute::Normal,   task.vertices, 3},
            {GeometryBuffer::Attribute::TexCoord, task.vertices, 2} },
            8 * sizeof(float), task.indices);

        task.mesh->setReady();

        localQueue.pop();
    }
}

void AssetLoadManager::bindToLua(sol::state& lua)
{
    std::cout << "AssetLoadManager::bindToLua() called!" << std::endl;

    //lua.new_usertype<AssetLoadManager>("AssetLoadManager",
    //    "waitForAllPendingLoads", &AssetLoadManager::waitForAllPendingLoads
    //);
    lua["AssetLoadManager"] = std::ref(AssetLoadManager::instance());

    std::cout << "AssetLoadManager bound to Lua successfully!" << std::endl;
}

/**
 * $BRIEF
 * helper process mesh data
 * \param mesh
 * \return \b 
 */
std::pair<std::vector<float>, std::vector<unsigned int>>
AssetLoadManager::processMeshData(const aiMesh* mesh)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Pre-allocate memory for better performance
    vertices.reserve(mesh->mNumVertices * 8); // 3 pos + 3 normal + 2 texcoord
    indices.reserve(mesh->mNumFaces * 3);     // Assuming triangles

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        // Position
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        // Normal
        if (mesh->HasNormals()) {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        }
        else {
            vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f });
        }

        // TexCoord
        if (mesh->HasTextureCoords(0)) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            vertices.insert(vertices.end(), { 0.0f, 0.0f });
        }

    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
    }

    return { vertices, indices };
}

/**
 * $BRIEF
 * are all thread completet load, I dont kind if anyone need it, just put here incase
 * \return \b 
 */
bool AssetLoadManager::areAllLoadsComplete() const
{
    std::lock_guard<std::mutex> lock(futuresMutex_);
    for (auto& future : pendingLoads_) {
        if (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            return false;
        }
    }

    // Also check if GPU upload queue is empty
    std::lock_guard<std::mutex> uploadLock(uploadMutex_);
    return uploadQueue_.empty();
}



std::shared_ptr<Mesh> AssetLoadManager::processMeshSecond(const aiMesh* mesh)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        // Position
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        //vertices.push_back(1.0f);

        // Normal
        if (mesh->HasNormals())
        {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        // Texture Coordinates
        if (mesh->HasTextureCoords(0))
        {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        // Tangents
        if (mesh->HasTangentsAndBitangents())
        {
            vertices.push_back(mesh->mTangents[i].x);
            vertices.push_back(mesh->mTangents[i].y);
            vertices.push_back(mesh->mTangents[i].z);
        }
        else
        {
            // Optional: compute tangents manually later if needed
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    std::cout << "Loaded mesh: " << mesh->mNumVertices << " vertices" << std::endl;

    auto mesh_ = std::make_shared<Mesh>(file_path);
    mesh_->setVertexData({ {GeometryBuffer::Attribute::Position, vertices, 3},
                          {GeometryBuffer::Attribute::Normal, vertices, 3},
                          {GeometryBuffer::Attribute::TexCoord, vertices, 2},
                          {GeometryBuffer::Attribute::Tangent, vertices, 3} },
        11 * sizeof(float), indices);
    ResourceManager<Mesh>::create(file_path, mesh_);

    return mesh_;

}





/**
 * @brief Reads the contents of a shader file from disk.
 *
 * This function opens the given file, reads its contents into a string,
 * and returns it. It is typically used for loading GLSL shader source code.
 *
 * @param filePath Path to the shader file.
 * @return std::string Contents of the file as a single string.
 *
 * @throws std::runtime_error If the file cannot be opened or read.
 */
std::string AssetLoadManager::readShaderFile(const std::string &filePath)
{
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open())
    {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    return shaderStream.str();
}



/**
 * @brief Loads a texture image from a file or retrieves it from the resource manager.
 *
 * This function first checks whether the specified texture has already been loaded
 * by querying the ResourceManager. If the texture exists, the existing shared pointer
 * is returned to avoid redundant loading. Otherwise, a new texture is created using
 * the given filename, stored in the ResourceManager, and then returned.
 *
 * @param filename The file path or name of the texture image to load.
 * @return std::shared_ptr<Texture> A shared pointer to the loaded or retrieved texture.
 *
 * @note This function ensures that texture resources are reused efficiently and
 *       not reloaded multiple times.
 */
std::shared_ptr<Texture> AssetLoadManager::loadTextureImage(const std::string& filename) {
    // add resource manager
    auto ptr = ResourceManager<Texture>::retrieve(filename);
    if (ptr != nullptr) return ptr;
    auto tx = Texture::create(filename);
    ResourceManager<Texture>::create(filename, tx);
    return tx;
}




/**
 * @brief Loads a texture asset from the given file path.
 *
 * This function handles loading image data (e.g., diffuse, normal, or specular maps)
 * from the specified file and prepares it for use in rendering.
 *
 * @param Texturepath Path to the texture file to be loaded.
 */
std::shared_ptr<Texture> AssetLoadManager::assetTexture(const char *Texturepath)
{
    // add resource manager
    std::cout << "Texture being read from path: " << Texturepath << "\n";
    auto ptr = ResourceManager<Texture>::retrieve(Texturepath);
    if (ptr != nullptr)
        return ptr;
    auto tx = Texture::create(Texturepath);
    ResourceManager<Texture>::create(Texturepath, tx);
    return tx;
}

/**
 * @brief Constructs a new AssetLoadManager object.
 *
 * Initializes any resources or states required for asset management.
 * This constructor is private/protected when using the singleton pattern,
 * so it cannot be called directly outside the class.
 */
AssetLoadManager::AssetLoadManager()
{
}

/**
 * @brief Destroys the AssetLoadManager object.
 *
 * Cleans up and releases any resources held by the asset manager.
 * Called automatically when the singleton instance is destroyed.
 */
AssetLoadManager::~AssetLoadManager()
{
}
