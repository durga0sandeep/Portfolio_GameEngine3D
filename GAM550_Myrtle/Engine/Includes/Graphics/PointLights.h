#pragma once
#include "pch.h"

class PointLights
{
private:

    struct PointLightData
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 intensity;
        float radius;
    };

    std::vector<PointLightData> pointLights;


public:

    void createPointLights(bool many);//for now use false
    glm::vec3 RandomColor();
    glm::vec3 RandomIntensity();
    int RandomRadius();


};


