/*!****************************************************************************
 * file  : PlayerController.h
 * brief :
 *          This component enables input, attaches the camera
 *          to its owner. It also decides on whether the view
 *          is third person or first person (other views are possible)
 *          if the owner has a skeletal mesh component, it also helps
 *          manipulate the animation clips playing based on button presses
 *          If physics is implemented on the owner, it also allows the
 *          owner to jump + interact with other objects with a phy component
 *          in the scene.
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : November 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#pragma once

#include "Component.h"
#include "Graphics/Renderer.h"
#include "Entities/GameObject.h"
#include "Input/Input.h"
#include "Components/PhysicsComponent.h"
#include "SkeletalMeshComponent.h"

enum Perspective
{
    FIRST_PERSON,
    THIRD_PERSON
};

enum class MovementState
{
    IDLE,
    WALKING,
    RUNNING,
    JUMPING,
    FALLING
};

class PlayerController : public Component
{
private:
    // Camera
    Perspective         perspective;
    Camera              &cam;
    float               cameraDistance;
    float               cameraPitch;
    float               cameraYaw;
    float               cameraSensitivity;
    float               firstPersonYaw;     // Horizontal look
    float               firstPersonPitch;   // Vertical look

    // Movement
    float               moveSpeed;
    float               runSpeedMultiplier;
    glm::vec3           currentmovement;
    float               jumpForce = 100;
    glm::vec3           moveDirection;
    MovementState       currentState;
    MovementState       previousState;

    // Components
    PhysicsComponent        *physicsComp;
    SkeletalMeshComponent   *skeletalMesh;

    // Input state
    bool                isGrounded;
    bool                isRunning;

    // Animation names (configurable)
    std::string         idleAnimName;
    std::string         walkAnimName;
    std::string         runAnimName;
    std::string         jumpAnimName;

    //bool wasToggleKeyPressed;
    bool wasViewTogglePressed;
    bool wasActionPressed;

public:
    PlayerController(GameObject* _owner, Perspective p);
    static void registerWithFactory(sol::state& lua);
    static void bindToLua(sol::state& lua);
    void        update() override;

    ////////////////////////////
    /// setters:
    void    setPerspective(Perspective p);
    void    setCameraDistance(float distance) { cameraDistance = distance; }
    void    setCameraSensitivity(float sensitivity) { cameraSensitivity = sensitivity; }
    void    setMoveSpeed(float speed) { moveSpeed = speed; }
    void    setRunSpeedMultiplier(float multiplier) { runSpeedMultiplier = multiplier;  }
    void    setJumpForce(float force) { jumpForce = force; }
    void    setAnimationNames( const std::string& idle,
                            const std::string& walk,
                            const std::string& run,
                            const std::string& jump );

    ////////////////////////////
    /// Getter:
    //MovementState getMovementState() const;// { return currentState; }
    bool    getActionPressed() { return wasActionPressed; }

private:
    void updateCamera();
    void updateMovement();
    void updateJump();
    void updateAnimation();
    glm::vec3 calculateMoveDirection();
    void updateMovementState();
    void applyCamera();
};

