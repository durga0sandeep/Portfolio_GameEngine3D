/* *
Copyright(c) 2025 Digipen Institute of Technology
Author : Martin Chow
*/
#include "pch.h"
#include "Entities/Camera.h"


glm::mat4 Camera::getView() const
{
    return viewMatrix;
}

glm::mat4 Camera::getPerspective() const
{
    return perspectiveMatrix;
}

// TODO: Updated the camera logic, so need a new way of doing this
glm::mat4 Camera::swtich2D_3D(float time)
{
    // reminder, I use https://learnwebgl.brown37.net/index.html chapter 7 and 8 for this equation
    // need to update if there is other game mec
    if (camMode == CameraMode::CAMERA_MODE_2D) {
        float hmid = (left + right) / 2;
        float wmid = (bottom + top) / 2;
        eye.x = hmid;
        eye.y = wmid;
        /*center.x = hmid;
        center.y = wmid;
        center.z = eye.z - camdis;*/
        // up not change for now, but if up is going to to change should be recalculte
        // eye z(forward) should not change in 2D or maybe later we want to change or some other kind of ingame mech
        // like rotation on cam pos? then we need to change the formula
        // but none the less, it should use the camdis saved
    }
    else {
        // should move camdis to here, originally is precalculate
        // add x y in case there is tile on angle
        /*camdis = pow((eye.x - center.x), 2) + pow((eye.y - center.y), 2) + pow((eye.z - center.z), 2);*/
        camdis = float(pow(camdis, 0.5));
        left = eye.x - camdis;
        right = eye.x + camdis;
        top = (eye.y + camdis / aspectRatio);
        bottom = (eye.y - camdis / aspectRatio);
    }
    // glm::mat4 result =startmat* dt + end * (1 - dt);
    return glm::mat4(1.0f);
}

void Camera::setEye(glm::vec3 pos)
{
    eye = pos;
}

glm::vec3 Camera::getEye()
{
    return eye;
}

void Camera::setTarget(glm::vec3 pos)
{
    target = pos;
}

glm::vec3 Camera::getTarget()
{
    return target;
}

void Camera::setThirdPersonAngle(float _yaw, float _pitch)
{
    thirdPersonYaw = _yaw;
    thirdPersonPitch = _pitch;
}

void Camera::setthirdPersonDistance(float dis)
{
    thirdPersonDistance = dis;
}

void Camera::reset()
{
    // important this need to be very long
    far = 10000.0f;
    fovy = static_cast<float>(75 * 3.14159 / 180);
    camdis = 10.0f;

    // for ingame nav and game dev mode in 3D
    tilt = 0.0f;
    spin = 0.0f;

    //std::cout << "Window resized to " << width << "x" << height << std::endl;
    //aspectRatio = static_cast<float>(width) / height;
    target = glm::vec3(0.0f);         // Target position (typically the player)
    //thirdPersonDistance = 1000.0f;           // Distance from target
    thirdPersonOffset = glm::vec3(0.0f, 0.0f, 0.0f);  // Offset from target (typically slightly above)
    thirdPersonPitch = -30.0f;              // Up/down angle
    thirdPersonYaw = 0.0f;                // Left/right angle
    // Smoothness or you can call it turn speed
    thirdPersonSmoothness = 0.2f;         // Lower values = more responsive camera
}


void Camera::setMode(CameraMode cameraMode)
{
    camMode = cameraMode;
}

void Camera::update()
{
    switch (camMode) {
    case CameraMode::CAMERA_MODE_2D:
        // 2D mode
        setMode2DMatrix();
        break;
    case CameraMode::CAMERA_MODE_3D:
        // 3D mode
        setMode3DMatrix();
        break;
    case CameraMode::CAMERA_MODE_BOUND_TO_USER:
        // Another condition
        setModeUserBoundMatrix();
        break;
    case CameraMode::CAMERA_MODE_THIRD_PERSON:
        // Another condition
        setModeThridPerson();
        break;
    default:
        // Default case
        // Do nothing for now
        break;
    }
}

float Camera::getYaw()
{
    return thirdPersonYaw;
}

float Camera::getPitch()
{
    return thirdPersonPitch;
}

// using orthogrpahic
void Camera::setMode2DMatrix()
{
    viewMatrix = glm::mat4(1.0f);// identity matrix
    perspectiveMatrix = glm::ortho(left, right, bottom, top, near, far);
}

// using look at
void Camera::setMode3DMatrix()
{
    viewMatrix = glm::lookAt(eye, target, up);
    perspectiveMatrix = glm::perspective(fovy, aspectRatio, near, far);
}

// Bound with user mouse, or other way
void Camera::setModeUserBoundMatrix()
{
    perspectiveMatrix = glm::perspective(fovy, aspectRatio, near, far);

    // Calculate look direction from yaw and pitch
    float yawRad = glm::radians(spin);
    float pitchRad = glm::radians(tilt);

    glm::vec3 forward;
    forward.x = cos(pitchRad) * sin(yawRad);
    forward.y = sin(pitchRad);
    forward.z = cos(pitchRad) * cos(yawRad);
    forward = glm::normalize(forward);

    glm::vec3 lookAtPoint = eye + forward;

    viewMatrix = glm::lookAt(eye, lookAtPoint, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::setModeThridPerson()
{
    // Correct spherical to Cartesian conversion
    // Yaw rotates around Y axis (horizontal rotation)
    // Pitch rotates up/down
    float yawRad = static_cast<float>(degreeToRadian(thirdPersonYaw));
    float pitchRad = static_cast<float>(degreeToRadian(thirdPersonPitch));

    float x = thirdPersonDistance * cos(pitchRad) * sin(yawRad);
    float y = thirdPersonDistance * sin(pitchRad);
    float z = thirdPersonDistance * cos(pitchRad) * cos(yawRad);

    glm::vec3 offset(x, y, z);

    // Camera position relative to target (player)
    glm::vec3 desiredEye = target + offset;

    // Apply smoothing for smooth camera movement
    if (thirdPersonSmoothness > 0.0f) {
        eye = glm::mix(eye, desiredEye, 1.0f - thirdPersonSmoothness);
    }
    else {
        eye = desiredEye;
    }

    // Camera always looks at target (with optional offset for looking above player's head)
    glm::vec3 lookAtPoint = target + thirdPersonOffset;

    // Update orientation vector (direction camera is looking)
    orientation = glm::normalize(lookAtPoint - eye);

    // Update matrices
    viewMatrix = glm::lookAt(eye, lookAtPoint, up);
    perspectiveMatrix = glm::perspective(fovy, aspectRatio, near, far);
}

double Camera::degreeToRadian(double degrees)
{
    return degrees * (3.14 / 180.0);
}


void Camera::initializeCam(){
    setMode(CameraMode::CAMERA_MODE_3D);
    eye = glm::vec3(12.0f, 5.0f, 30.0f);
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);
    target = glm::vec3(0,0,0);

    up = upVector;
    float windowWidth = 800;
    float windowHeight = 600;
    aspectRatio = windowWidth / windowHeight;
    fovy = glm::radians(45.0f);
    float bottom = -windowHeight * 0.5f;
    bottom = bottom;
    float top = windowHeight * 0.5f;
    top = top;
    float left = -windowWidth * 0.5f;
    left = left;
    float right = windowWidth * 0.5f;
    right = right;

    near = 0.1f;
    far = 10000.0f;
}


