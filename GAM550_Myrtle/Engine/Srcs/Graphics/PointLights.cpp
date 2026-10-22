#include "pch.h"
#include "Graphics/PointLights.h"


void PointLights::createPointLights(bool many)
{
    unsigned int i = 0;
    unsigned int j = 0;

    float  Xpos = -20.0f;
    float Ypos = -20.0f;
    float temp = Ypos;

    if (many == true)
    {

        for (i = 0; i < 40; i++)
        {
            for (j = 0; j < 25; j++)
            {

                PointLightData LightsData;

                LightsData.position = glm::vec3(Xpos, Ypos, 0);
                LightsData.color = RandomColor();
                LightsData.intensity = RandomIntensity();
                LightsData.radius = RandomRadius();

                pointLights.push_back(LightsData);

                Ypos += 1.5f;
            }
            Xpos += 1.5f;
            Ypos = temp;

        }

    }
    else if (many == false)
    {

        Xpos = 0, Ypos = 0;

        for (int i = 0; i < 4; i++) {

            PointLightData LightsData;

            LightsData.position = glm::vec3(Xpos, Ypos, 0);
            LightsData.color = RandomColor();
            LightsData.intensity = RandomIntensity();
            LightsData.radius = RandomRadius();

            pointLights.push_back(LightsData);

            Xpos += 1;
            Ypos += 1;

        }
    }

}



glm::vec3 PointLights::RandomColor() {

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0f, 1.0f);

    float r = dis(gen);
    float g = dis(gen);
    float b = dis(gen);

    // Round to 1 decimal place
    r = std::round(r * 10.0f) / 10.0f;
    g = std::round(g * 10.0f) / 10.0f;
    b = std::round(b * 10.0f) / 10.0f;

    return glm::vec3(r, g, b);
}



glm::vec3 PointLights::RandomIntensity() {

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.5f, 3.0f);

    float r = dis(gen);
    float g = dis(gen);
    float b = dis(gen);

    // Round to 2 decimal places
    r = std::round(r * 100.0f) / 100.0f;
    g = std::round(g * 100.0f) / 100.0f;
    b = std::round(b * 100.0f) / 100.0f;

    return glm::vec3(r, g, b);
}


int PointLights::RandomRadius() {

    const int minRadius = 3, maxRadius = 4;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(minRadius, maxRadius);

    return dis(gen);
}















