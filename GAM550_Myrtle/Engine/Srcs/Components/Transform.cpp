/* *
Copyright(c) 2025 Digipen Institute of Technology
Author : Martin Chow
*/
#include "pch.h"
#include "Components/Transform.h"

Transform::Transform()
    : position(0, 0, 0), rotation(1, 0, 0, 0), cached_euler_rotation(0, 0, 0), scaling(1, 1, 1) {}

void Transform::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

const glm::vec3& Transform::getPosition() const {
    return position;
}

void Transform::setRotation(const glm::vec3& newRotation) {
    rotation = glm::quat(newRotation);
    cached_euler_rotation = newRotation;
}

const glm::vec3& Transform::getRotation() const {
    return cached_euler_rotation;
}

const glm::quat& Transform::getQuatRotation() const {
    return rotation;
}

void Transform::setForward(const glm::vec3& up, const glm::vec3& forward)
{
    glm::quat q = glm::quat(up, forward);
    rotation = glm::eulerAngles(q);
}

void Transform::setRotation(const glm::quat& newRotation)
{
    rotation = glm::eulerAngles(newRotation);
}

void Transform::setScale(const glm::vec3& newScale) {
    scaling = newScale;
}

const glm::vec3& Transform::getScale() const {
    return scaling;
}

void Transform::translate(const glm::vec3& translation) {
    position = position + translation;
}

void Transform::rotate(const glm::vec3& rotationDelta) {
    // Create a quaternion from the rotation delta
    glm::quat deltaQuat = glm::quat(rotationDelta);

    // Apply the rotation (quaternion multiplication)
    rotation = deltaQuat * rotation;

    // Update cached euler rotation
    cached_euler_rotation = glm::eulerAngles(rotation);
}

void Transform::scale(const glm::vec3& scale) {
    scaling.x = scaling.x * scale.x;
    scaling.y = scaling.y * scale.y;
    scaling.z = scaling.z * scale.z;
}


glm::mat4 Transform::getLocalMatrix() {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

    glm::mat4 rotationMatrix = getRotationMatrix();

    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scaling);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::mat4 Transform::getRotationMatrix() {
    /*glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;
    return rotationMatrix;*/
    return glm::mat4_cast(rotation);
}

glm::quat Transform::getquatrotation()
{
    return glm::quat();
}

glm::mat4 Transform::getTranslateMatrix() {
    return glm::translate(glm::mat4(1.0f), position);
}

glm::mat4 Transform::getScaleMatrix() {
    return glm::scale(glm::mat4(1.0f), scaling);
}

glm::mat4 Transform::getInverseLocalMatrix() {
    return glm::inverse(getLocalMatrix());
}
