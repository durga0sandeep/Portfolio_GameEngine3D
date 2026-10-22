/*!****************************************************************************
 * file  : @file
 * brief : Emiiter that spawns and manages particles
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include <glm/ext/vector_float3.hpp>
#include <Graphics/Texture.h>


struct Particle {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);
    float size = 1.0f;
    float life = 0.0f;
    float maxLife = 0.5f;

    int frameIndex = 0; // Texture atlas frame
    int totalFrames = 1; // Total frames in animation
};

struct ParticleInstanceData {
    glm::vec3 position;
    float size;
    int frameIndex;
    int totalFrames;
};

class ParticleEmitter {
public:
    void update(float dt);
    void render(const glm::mat4& view, const glm::mat4& proj);
    void emit(); // spawn particles
    void setPosition(const glm::vec3& pos);
    void setTexture(std::shared_ptr<Texture> tex); // Set texture for this emitter
    void setAtlas(int totalFrames); // For animated particles
    void sortParticles(const glm::vec3& cameraPos);
    ParticleEmitter();

private:
    std::vector<Particle> particles;
    std::vector<ParticleInstanceData> instanceData;
    // Optional: emitter-specific shader or mesh

    glm::vec3 emitterPosition = glm::vec3(0.0f);
    std::shared_ptr<Texture> texture = nullptr;
    int totalFrames = 1;
    int maxParticles = 100;
    GLuint instanceVBO;

    void updateInstanceBuffer();



};

