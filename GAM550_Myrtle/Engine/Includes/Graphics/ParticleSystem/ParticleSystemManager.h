/*!****************************************************************************
 * file  : @file
 * brief : Manager that handles multiple particle emitters and shared resources
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include <vector>
#include <memory>
#include <Graphics/Shader.h>
#include <glm/glm.hpp>        // For vec3 and mat4
#include <glm/gtc/matrix_transform.hpp> // For transformations like translate, rotate, scale
//#include <glm/gtc/type_ptr.hpp>         // For converting to pointer types (e.g., for OpenGL)
#include "Graphics/ParticleSystem/ParticleEmitter.h"

class ParticleSystemManager {
public:
    // Singleton access
    static ParticleSystemManager& instance();

    // Initialization
    void initializeSharedResources();

    // Emitter management
    void addEmitter(std::shared_ptr<ParticleEmitter> emitter);
    void update(float dt);
    void render(const glm::mat4& view, const glm::mat4& proj);

    // Shared resource access
    GLuint getQuadVAO() const;
    std::shared_ptr<Shader> getSharedShader() const;

private:
    // Shared quad geometry
    GLuint quadVAO = 0;
    GLuint quadVBO = 0;
    GLuint quadEBO = 0;

    // Shared shader
    std::shared_ptr<Shader> shader;
    const char* shaderPath = "Shader/particles_vertex.glsl\nShader/particles_fragment.glsl";

    // Emitters
    std::vector<std::shared_ptr<ParticleEmitter>> emitters;

    // Singleton enforcement
    ParticleSystemManager();
    ParticleSystemManager(const ParticleSystemManager&) = delete;
    ParticleSystemManager& operator=(const ParticleSystemManager&) = delete;
};
