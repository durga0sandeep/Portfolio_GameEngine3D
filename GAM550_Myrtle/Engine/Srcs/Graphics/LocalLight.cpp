#include "pch.h"
#include "Graphics/LocalLight.h"

void LocalLight::setRotation(size_t index, const glm::vec3& newRotation)
{
    if (index >= Rotations.size()) {
        throw std::out_of_range("Rotation index out of range");
    }
    Rotations[index] = newRotation;
}

int LocalLight::structSize()
{
    return sizeof(pointLightData);
}

int LocalLight::listDataSize()
{
    return PointLights.size();
}

int LocalLight::CreateLights(glm::vec3 pos, glm::vec3 color, float intensity, float radius, std::shared_ptr<Mesh> mesh, glm::vec3 scale, glm::vec3 rotation)
{
    pointLightData pLight;
    pLight.position = pos;
    pLight.color = color;
    pLight.intensity = intensity;
    pLight.radius = radius;

    PointLights.push_back(pLight);
    AllMeshs.push_back(mesh);
    Scales.push_back(scale);
    Rotations.push_back(rotation);
    return PointLights.size() - 1;
}

pointLightData& LocalLight::GetPointLight(size_t index)
{
    if (index >= PointLights.size()) {
        throw std::out_of_range("Point light index out of range");
    }
    return PointLights[index];
}

std::shared_ptr<Mesh>& LocalLight::getMesh(size_t index)
{
    if (index >= AllMeshs.size()) {
        throw std::out_of_range("Mesh index out of range");
    }
    return AllMeshs[index];
}

glm::vec3& LocalLight::GetScale(size_t index)
{
    if (index >= Scales.size()) {
        throw std::out_of_range("Scale index out of range");
    }
    return Scales[index];
}

glm::vec3& LocalLight::GetRotation(size_t index)
{
    if (index >= Rotations.size()) {
        throw std::out_of_range("Rotation index out of range");
    }
    return Rotations[index];
}

void LocalLight::setScale(size_t index, const glm::vec3& newScale)
{
    if (index >= Scales.size()) {
        throw std::out_of_range("Scale index out of range");
    }
    Scales[index] = newScale;
}


