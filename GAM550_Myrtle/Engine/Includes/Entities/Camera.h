/*!****************************************************************************
 * file  : Camera.h
 * brief : Camera - This class provides the core functionality for camera operations.
 * It is intended for read-only use; modifications should be done
 * through the Camera Controller.
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
#include <glad/glad.h>

/**
 * @brief Undefine macros `near` and `far` to avoid conflicts with member variables.
 */
#undef near
#undef far

/**
  * @enum CameraMode
  * @brief Defines the different operational modes for the Camera.
  */
enum CameraMode
{
    CAMERA_MODE_2D,             /**< 2D camera mode. */
    CAMERA_MODE_3D,             /**< 3D camera mode. */
    CAMERA_MODE_BOUND_TO_USER,  /**< Camera follows a user or object. */
    CAMERA_MODE_THIRD_PERSON    /**< Third-person camera view. */
};

/**
 * @class Camera
 * @brief Represents a camera in 2D, 3D, or third-person view.
 *
 * The Camera class manages the view and projection matrices, allowing
 * switching between 2D and 3D modes, third-person tracking, and custom
 * user-bound camera behavior. It also handles camera rotation, position,
 * target following, and projection parameters.
 */
class Camera
{
public:
    /**
    * @brief Returns the camera's view matrix.
    * @return glm::mat4 The view matrix representing the camera's position and orientation.
    */
    glm::mat4 getView() const;

    /**
     * @brief Returns the camera's perspective (projection) matrix.
     * @return glm::mat4 The perspective projection matrix.
     */
    glm::mat4 getPerspective() const;

    /**
     * @brief Smoothly switches the camera between 2D and 3D modes over time.
     * @param time Interpolation factor or time step for the transition.
     * @return glm::mat4 The resulting view or projection matrix during the transition.
     */
    glm::mat4 swtich2D_3D(float time);

    /**
     * @brief Sets the orientation of the camera.
     * @param orient A vector representing the new orientation (e.g., pitch, yaw, roll).
     */
    void setOrientation(const glm::vec3& orient) { orientation = orient; }

    /**
     * @brief Sets the position (eye) of the camera.
     * @param pos The new camera position in world space.
     */
    void setEye(glm::vec3 pos);

    /**
     * @brief Gets the current position (eye) of the camera.
     * @return glm::vec3 The camera's position in world space.
     */
    glm::vec3 getEye();



    // this is only use when third person view
    /**
     * @brief Sets the target position the camera should follow.
     * @note Only used in third-person view mode.
     * @param pos The target position in world space.
     */
    void setTarget(glm::vec3 pos);

    /**
     * @brief Gets the current target position of the camera.
     * @return glm::vec3 The target position in world space.
     */
    glm::vec3 getTarget();

    /**
     * @brief Sets the yaw and pitch angles for third-person camera orientation.
     * @param yaw Rotation around the Y-axis (horizontal rotation).
     * @param pitch Rotation around the X-axis (vertical rotation).
     */
    void setThirdPersonAngle(float yaw, float pitch);


    /**
     * @brief Sets the distance between the camera and its target in third-person view.
     * @param dis Desired distance from the target.
     */
    void setthirdPersonDistance(float dis);

    /**
     * @brief Resets the camera to its default position, orientation, and mode.
     */
    void reset();


    /**
     * @brief Sets the camera's operating mode.
     * @param mode The desired CameraMode (e.g., 2D, 3D, third-person).
     */
    void setMode(CameraMode);

    /**
     * @brief Updates the camera's matrices and internal state.
     *
     * Call this every frame to ensure the camera correctly reflects
     * its position, orientation, and mode changes.
     */
    void update();

    /**
     * @brief Gets the current yaw (rotation around Y-axis) of the camera.
     * @return float Yaw angle in degrees.
     */
    float getYaw();

    /**
     * @brief Gets the current pitch (rotation around X-axis) of the camera.
     * @return float Pitch angle in degrees.
     */
    float getPitch();


    void initializeCam( );

    
    /** @name Projection parameters (for 2D or 3D mode) */
    ///@{
    float left = 0;          /**< Left boundary of the view frustum (orthographic). */
    float right = 0;          /**< Right boundary of the view frustum (orthographic). */
    float bottom = 0;          /**< Bottom boundary of the view frustum (orthographic). */
    float top = 0;          /**< Top boundary of the view frustum (orthographic). */
    float near = 0.1f;       /**< Near clipping plane distance. */
    float far = 10000.0f;   /**< Far clipping plane distance. Must be large for 3D navigation. */
    float fovy = 0;          /**< Field of view in the Y direction (for perspective). */
    float camdis = 10.0f;      /**< Default camera distance (for third-person or perspective view). */
    ///@}

    /** @name Camera rotation parameters (for in-game navigation or 3D dev mode) */
    ///@{
    float tilt = 0.0f;         /**< Tilt angle for camera rotation. */
    float spin = 0.0f;         /**< Spin angle for camera rotation. */
    ///@}

    /** @brief Camera's position in world space. */
    glm::vec3 eye = glm::vec3();

    /** @brief Camera's orientation vector (default looking down -Z). */
    glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);

    /** @brief Up direction vector for the camera. */
    glm::vec3 up = glm::vec3();

    /** @brief Aspect ratio of the camera (width / height). */
    float aspectRatio;

    // use to set the camera mode to different kind of matrix
    CameraMode camMode;
   
private:
    /** @name Third-Person Camera Parameters */
    ///@{
    glm::vec3 target = glm::vec3(0.0f);               /**< Target position the camera follows (typically the player). */
    float thirdPersonDistance = 100.0f;              /**< Distance from the target. */
    glm::vec3 thirdPersonOffset = glm::vec3(0.0f, 5.0f, 0.0f); /**< Offset from target (e.g., slightly above). */
    float thirdPersonPitch = -30.0f;                 /**< Up/down angle for third-person view. */
    float thirdPersonYaw = 0.0f;                     /**< Left/right angle for third-person view. */
    float thirdPersonSmoothness = 0.2f;              /**< Camera smoothness / turn speed. Lower values = more responsive. */
    ///@}

    /** @brief View matrix of the camera. */
    glm::mat4 viewMatrix;

    /** @brief Perspective (projection) matrix of the camera. */
    glm::mat4 perspectiveMatrix;

    /**
     * @brief Sets the camera matrices for 2D mode.
     */
    void setMode2DMatrix();

    /**
     * @brief Sets the camera matrices for 3D mode.
     */
    void setMode3DMatrix();

    /**
     * @brief Sets the camera matrices for a user-bound mode.
     */
    void setModeUserBoundMatrix();

    /**
     * @brief Sets the camera matrices for third-person mode.
     */
    void setModeThridPerson();

    /**
     * @brief Converts degrees to radians.
     * @param degrees Angle in degrees.
     * @return double Angle in radians.
     */
    double degreeToRadian(double degrees);

};

