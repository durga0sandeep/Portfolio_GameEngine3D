/*!****************************************************************************
 * file  : Transform.h
 * brief : Transform - The Transform class stores translation, rotation (Euler angles and quaternion),
 * and scale of an object, and provides methods to get transformation matrices
 * for rendering or physics calculations.
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


 /**
  * @class Transform
  * @brief Represents position, rotation, and scale of an object in 3D space.
  *
  * Provides utilities to manipulate and query spatial transformations,
  * including translation, Euler/quaternion rotations, scaling, and
  * constructing transformation matrices.
  */
class Transform
{
public:
    /** @brief Default constructor initializing position, rotation, and scale. */
    Transform();

    /** @name Position Methods */
    ///@{
    void setPosition(const glm::vec3& newPosition); /**< Sets object position. */
    const glm::vec3& getPosition() const;           /**< Gets object position. */
    ///@}

    /** @name Rotation Methods (Euler angles in radians) */
    ///@{
    void setRotation(const glm::vec3& newRotation);  /**< Sets rotation using Euler angles. */
    const glm::vec3& getRotation() const;            /**< Gets rotation as Euler angles. */
    void setRotation(const glm::quat& newRotation);  /**< Sets rotation using quaternion. */
    const glm::quat& getQuatRotation() const;        /**< Gets rotation as quaternion. */
    glm::quat getquatrotation();                      /**< Returns rotation quaternion. */
    void setForward(const glm::vec3& up, const glm::vec3& forward); /**< Sets rotation to look in a given forward direction. */
    glm::mat4 getRotationMatrix();                    /**< Returns 3x3 rotation matrix. */
    ///@}

    /** @name Scale Methods */
    ///@{
    void setScale(const glm::vec3& newScale); /**< Sets object scale. */
    const glm::vec3& getScale() const;        /**< Gets object scale. */
    ///@}

    /** @name Transformation Matrices */
    ///@{
    glm::mat4 getLocalMatrix();          /**< Returns the model matrix (translation * rotation * scale). */
    glm::mat4 getInverseLocalMatrix();   /**< Returns the inverse of the model matrix. */
    glm::mat4 getTranslateMatrix();      /**< Returns translation matrix. */
    glm::mat4 getScaleMatrix();          /**< Returns scale matrix. */
    ///@}

    /** @name Utility Methods */
    ///@{
    void translate(const glm::vec3& translation); /**< Adds translation to the current position. */
    void rotate(const glm::vec3& rotationDelta);  /**< Adds rotation delta to the current rotation. */
    void scale(const glm::vec3& scaling);         /**< Multiplies the current scale by scaling vector. */
    ///@}

private:
    glm::vec3 position;              /**< Object position in world space. */
    glm::vec3 cached_euler_rotation;   /**< Cached Euler angles for rotation. */
    glm::quat rotation;              /**< Quaternion representing rotation. */
    glm::vec3 scaling;               /**< Object scale. */
};
