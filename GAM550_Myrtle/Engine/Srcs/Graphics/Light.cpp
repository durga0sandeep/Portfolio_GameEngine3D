#include "pch.h"
#include "Graphics/Light.h"

/// \brief Constant for pi
const float PI = 3.14159f;

/// \brief Conversion factor from degrees to radians
const float rad = PI / 180.0f;

/**
 * \class Light
 * \brief Represents a point light in 3D space with spherical coordinates.
 *
 * The Light class stores the light's position calculated from
 * spin, tilt (in degrees), and distance from the origin.
 */
Light::Light() :lightPos(glm::vec3(lightDist* cos(lightSpin* rad)* sin(lightTilt* rad),
    lightDist* sin(lightSpin* rad)* sin(lightTilt* rad),
    lightDist* cos(lightTilt* rad)))
{
};

/**
 * \brief Constructs a Light with specific spin, tilt, and distance.
 * \param spin Angle around the Y-axis in degrees.
 * \param tilt Angle from the Z-axis in degrees.
 * \param dist Distance from the origin.
 *
 * Initializes the light's position based on spherical coordinates.
 */
Light::Light(int spin, int tilt, int dist) :lightSpin(spin), lightTilt(tilt), lightDist(dist), lightPos(glm::vec3(lightDist* cos(lightSpin* rad)* sin(lightTilt* rad),
    lightDist* sin(lightSpin* rad)* sin(lightTilt* rad),
    lightDist* cos(lightTilt* rad)))
{
};

/**
 * \brief Updates the light's position based on current spin, tilt, and distance.
 *
 * Should be called whenever spin, tilt, or distance is changed.
 */
void Light::update()
{
    lightPos = glm::vec3(lightDist * cos(lightSpin * rad) * sin(lightTilt * rad),
        lightDist * sin(lightSpin * rad) * sin(lightTilt * rad),
        lightDist * cos(lightTilt * rad));
}

/**
 * \brief Returns the current position of the light in world space.
 * \return glm::vec3 representing the light's position.
 */
glm::vec3 Light::getLightPos() const
{
    return lightPos;
}

/// Create a light view matrix (light looking at origin)
glm::mat4 Light::getLightViewMatrix() const
{
    glm::vec3 target = glm::vec3(0.0f);     // look at scene center
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    return glm::lookAt(lightPos, target, up);
}

/// Create an orthographic projection for directional-like shadows
glm::mat4 Light::getLightOrthoProjection(float size, float nearPlane, float farPlane) const
{
    return glm::ortho(-size, size, -size, size, nearPlane, farPlane);
}

/// Create a perspective projection for spotlight shadows
glm::mat4 Light::getLightPerspectiveProjection(float fovDegrees, float nearPlane, float farPlane) const
{
    return glm::perspective(glm::radians(fovDegrees), 1.0f, nearPlane, farPlane);
}


