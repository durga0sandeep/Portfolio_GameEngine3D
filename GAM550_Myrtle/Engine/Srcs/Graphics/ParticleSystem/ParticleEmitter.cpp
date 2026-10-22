#include "pch.h"
#include "Graphics/ParticleSystem/ParticleEmitter.h"
#include <Graphics/ParticleSystem/ParticleSystemManager.h>
#include <Graphics/Renderer.h>

void ParticleEmitter::update(float dt) {
    for (auto it = particles.begin(); it != particles.end(); ) {
        it->life += dt;
        if (it->life >= it->maxLife) {
            it = particles.erase(it); // Remove dead particle
        }
        else {
            // Update position
            it->position += it->velocity * dt;

            // Animate frame index based on life progress
            float lifeRatio = it->life / it->maxLife;
            it->frameIndex = static_cast<int>(lifeRatio * it->totalFrames);
            if (it->frameIndex >= it->totalFrames) {
                it->frameIndex = it->totalFrames - 1;
            }

            ++it;
        }
    }

    updateInstanceBuffer(); // Refresh GPU data
    sortParticles(Renderer::instance().camera.getEye());
}

void ParticleEmitter::render(const glm::mat4& view, const glm::mat4& proj)
{
    if (instanceData.empty()) return;

    auto& manager = ParticleSystemManager::instance();
    auto shader = manager.getSharedShader();

    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("proj", proj);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture ? texture->getID() : 0);
    shader->setInt("particleTexture", 0);

    // Bind shared quad VAO
    glBindVertexArray(manager.getQuadVAO());

    // NOW bind this emitter's instance buffer and set up attributes
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    // Upload instance data
    glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(ParticleInstanceData),
        instanceData.data(), GL_DYNAMIC_DRAW);

    // Set up instance attributes (do this every frame)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleInstanceData), (void*)0);
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleInstanceData),
        (void*)(offsetof(ParticleInstanceData, size)));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 1, GL_INT, sizeof(ParticleInstanceData),
        (void*)(offsetof(ParticleInstanceData, frameIndex)));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 1, GL_INT, sizeof(ParticleInstanceData),
        (void*)(offsetof(ParticleInstanceData, totalFrames)));
    glVertexAttribDivisor(5, 1);

    // Draw
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0,
        static_cast<GLsizei>(instanceData.size()));

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleEmitter::emit()
{
    Particle* p = nullptr;

    // Try to find a dead particle to reuse
    for (auto& particle : particles) {
        if (particle.life >= particle.maxLife) {
            p = &particle;
            break;
        }
    }

    // If no reusable particle and under cap, create new
    if (p == nullptr) {
        if (particles.size() >= maxParticles) {
            return; // Cap reached, skip emission
        }
        particles.emplace_back();
        p = &particles.back();
    }

    // Reset particle properties
    p->position = emitterPosition;

    float angle = static_cast<float>(rand()) / RAND_MAX * glm::pi<float>() * 2.0f;
    float speed = 1.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f;
    p->velocity = glm::vec3(cos(angle) * 0.5f, speed, sin(angle) * 0.5f);

    p->size = 0.5f + static_cast<float>(rand()) / RAND_MAX * 0.5f;
    p->life = 0.0f;
    p->maxLife = 1.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f;

    p->totalFrames = totalFrames;
    p->frameIndex = 0;
}

void ParticleEmitter::setPosition(const glm::vec3& pos)
{
    emitterPosition = pos;
}

void ParticleEmitter::setTexture(std::shared_ptr<Texture> tex)
{
    texture = tex;
}

void ParticleEmitter::setAtlas(int in_totalFrames)
{
    totalFrames = in_totalFrames;
}

ParticleEmitter::ParticleEmitter() {
    // Default configuration
    emitterPosition = glm::vec3(0.0f);
    totalFrames = 1;

    // Create instance buffer
    glGenBuffers(1, &instanceVBO);

    particles.reserve(maxParticles);
}

void ParticleEmitter::updateInstanceBuffer() {
    instanceData.clear();
    instanceData.reserve(particles.size());

    for (const auto& p : particles) {
        ParticleInstanceData data;
        data.position = p.position;
        data.size = p.size;
        data.frameIndex = p.frameIndex;
        data.totalFrames = p.totalFrames;
        instanceData.push_back(data);
    }

    /*glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(ParticleInstanceData), instanceData.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}

void ParticleEmitter::sortParticles(const glm::vec3& cameraPos)
{
    std::sort(particles.begin(), particles.end(), [&](const Particle& a, const Particle& b) {
        float distA = glm::length(a.position - cameraPos);
        float distB = glm::length(b.position - cameraPos);
        return distA > distB; // Back-to-front
        });
}
