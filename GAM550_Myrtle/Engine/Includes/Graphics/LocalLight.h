#pragma once
#include <vector>
#include "Graphics/Mesh.h"
#include <glm/glm.hpp>

struct pointLightData {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float radius;
    int pad[2];
};

class LocalLight
{
private:
    std::vector<pointLightData> PointLights;
    std::vector<std::shared_ptr<Mesh>> AllMeshs;

    // New vectors for scale and rotation
    std::vector<glm::vec3> Scales;
    std::vector<glm::vec3> Rotations;

public:
    LocalLight()
    {
        PointLights.reserve(100);
        AllMeshs.reserve(100);
        Scales.reserve(100);
        Rotations.reserve(100);
    };

    void reset() {
        PointLights.clear();
        AllMeshs.clear();
        Scales.clear();
        Rotations.clear();
    }

    int CreateLights(glm::vec3 pos,
        glm::vec3 color,
        float intensity,
        float radius,
        std::shared_ptr<Mesh> mesh,
        glm::vec3 scale = glm::vec3(2.0f),
        glm::vec3 rotation = glm::vec3(0.0f));

    std::vector<pointLightData>& GetPointLights() {
        return PointLights;
    }

    pointLightData& GetPointLight(size_t index);

    std::shared_ptr<Mesh>& getMesh(size_t index);

    // Accessors for scale and rotation
    glm::vec3& GetScale(size_t index);

    glm::vec3& GetRotation(size_t index);

    // Update methods
    void setScale(size_t index, const glm::vec3& newScale);

    void setRotation(size_t index, const glm::vec3& newRotation);

    int structSize();
    int listDataSize();
};
