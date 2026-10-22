#pragma once

#include <glm/glm.hpp>


/*!****************************************************************************
 * @class   Light
 * @brief   Represents a light source used in the rendering system.
 *          Manages light parameters such as spin, tilt, distance, position,
 *          and color. Provides functionality to update and retrieve the
 *          light's position based on its transformation settings.
 *
 * @details The Light class encapsulates properties controlling the light's
 *          orientation and color in the scene. It supports initialization
 *          with custom parameters and automatic recalculation of the light
 *          position when parameters are updated.
 *
 * @note    Default values are temporarily borrowed from the graphics class.
 *
 * @author  Durga Sandeep
 * @date    9/2025
 * @course  CS550
 * @copyright
 *          © 2025 DigiPen Institute of Technology. All rights reserved.
 *****************************************************************************/
class Light
{

public:

    int lightSpin = 90;
    int lightTilt = 90;
    int lightDist = 600;
    glm::vec3 lightPos;
    glm::vec3 lightColor = glm::vec3(1.5f, 1.5f, 1.5f); // also copying value from my graphic class for now
    Light();
    Light(int, int, int);
    Light(Light&) = default;

    // if there is any update to the data, the light pos need to recalculate
    void update();
    glm::vec3 getLightPos() const;


    glm::mat4 getLightViewMatrix() const;
    glm::mat4 getLightOrthoProjection(float size, float nearPlane, float farPlane) const;
    glm::mat4 getLightPerspectiveProjection(float fovDegrees, float nearPlane, float farPlane) const;



};





