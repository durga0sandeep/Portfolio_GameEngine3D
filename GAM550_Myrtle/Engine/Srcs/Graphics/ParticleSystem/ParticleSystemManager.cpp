#include "pch.h"
#include "Graphics/ParticleSystem/ParticleSystemManager.h"

void ParticleSystemManager::addEmitter(std::shared_ptr<ParticleEmitter> emitter)
{
    emitters.push_back(emitter);
}

void ParticleSystemManager::update(float dt)
{
    for (auto& emitter : emitters) {
        if (emitter) {
            emitter->update(dt);
        }
    }
}

void ParticleSystemManager::render(const glm::mat4& view, const glm::mat4& proj)
{
    if (!shader) return;

    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("proj", proj);

    for (auto& emitter : emitters) {
        if (emitter) {
            emitter->render(view, proj);
        }
    }
}

ParticleSystemManager::ParticleSystemManager() {
    // Define quad vertices and indices
    float vertices[] = {
        // positions     // UVs
        -0.5f, -0.5f,     0.0f, 0.0f,
         0.5f, -0.5f,     1.0f, 0.0f,
         0.5f,  0.5f,     1.0f, 1.0f,
        -0.5f,  0.5f,     0.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attribute 0: position (vec2)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // Attribute 1: UV (vec2)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
    initializeSharedResources();
}


ParticleSystemManager& ParticleSystemManager::instance()
{
    static ParticleSystemManager instance;
    return instance;
}

void ParticleSystemManager::initializeSharedResources()
{
    shader = std::make_shared<Shader>(shaderPath);
}

GLuint ParticleSystemManager::getQuadVAO() const {
    return quadVAO;
}

std::shared_ptr<Shader> ParticleSystemManager::getSharedShader() const
{
    return shader;
}
