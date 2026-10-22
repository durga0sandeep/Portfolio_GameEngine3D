/*!****************************************************************************
 * file  : PlayerController.cpp
 * brief : 
 * 
 * author: Salam Sibai
 * Course: GAM550
 * date  : November 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /****************************************************************************/

#include "pch.h"
#include "Components/PlayerController.h"
#include "Components/ComponentFactory.h"
#include "FRController/FramerateController.h"
#include "Entities/Scene.h"

PlayerController::PlayerController(GameObject* _owner, Perspective p)
    : Component(_owner), perspective(p), cam(Renderer::instance().camera)
    , cameraDistance(1000.0f), cameraPitch(20.0f), cameraYaw(0.0f),cameraSensitivity(0.1f)
    , firstPersonPitch(0.0f), firstPersonYaw(0.0f)
    , moveSpeed(5.0f), runSpeedMultiplier(2.0f), currentmovement(glm::vec3(0, 0, 0)), jumpForce(10.0f), moveDirection(0.0f)
    , currentState(MovementState::IDLE), previousState(MovementState::WALKING)
    , physicsComp(nullptr)
    , skeletalMesh(nullptr)
    , isGrounded(true)
    , isRunning(false)
    , idleAnimName("idle")
    , walkAnimName("walk")
    , runAnimName("run")
    , jumpAnimName("jump")
    , wasViewTogglePressed(false)
    , wasActionPressed(false)
{
    setOwner(_owner);
    if (auto phys = owner->FindComponent<PhysicsComponent>())
        physicsComp = phys.get();

    if (auto skel = owner->FindComponent<SkeletalMeshComponent>())
        skeletalMesh = skel.get();

    if (skeletalMesh)
    {
        skeletalMesh->playAnimClip(idleAnimName, true);
        std::cout << "Starting with idle animation\n";
    }

    cam.setMode(CAMERA_MODE_THIRD_PERSON);
    std::cout << "PlayerController created for " << owner->getName() << std::endl;
}

/**
 * $BRIEF   Changes camera view to follow the
 *          specified perspective
 * 
 * \param p perspective to change to
 */
void PlayerController::setPerspective(Perspective p)
{
    if (perspective != p)
    {
        perspective = p;

        // Update camera mode in renderer
        if (p == Perspective::FIRST_PERSON)
        {
            Renderer::instance().camera.setMode(CAMERA_MODE_BOUND_TO_USER);
        }
        else
        {
            Renderer::instance().camera.setMode(CAMERA_MODE_THIRD_PERSON);
        }
    }
}

void PlayerController::setAnimationNames(const std::string& idle, const std::string& walk,
    const std::string& run, const std::string& jump)
{
    idleAnimName = idle;
    walkAnimName = walk;
    runAnimName = run;
    jumpAnimName = jump;
}

void PlayerController::update()
{
    //bool change_view_key = Input::instance().IsKeyPressed(KeyCode::Backspace);
    bool end_level = Input::instance().IsKeyPressed(KeyCode::O);
    bool view_toggle = Input::instance().IsKeyPressed(KeyCode::P);
    bool action_key = Input::instance().IsKeyPressed(KeyCode::E);
    bool gamepad_action = Input::instance().IsGamepadButtonPressed(GamepadButton::X, 0);

    // Toggle perspective
    if (view_toggle && !wasViewTogglePressed)
    {
        if (perspective == Perspective::FIRST_PERSON)
            setPerspective(Perspective::THIRD_PERSON);
        else
            setPerspective(Perspective::FIRST_PERSON);
    }
    wasViewTogglePressed = view_toggle;

    // Action button
    if ((action_key || gamepad_action) && !wasActionPressed)
    {
        //performAction();
        std::cout << "Action key pressed!\n";
    }
    wasActionPressed = (action_key || gamepad_action);


    updateCamera();
    updateMovement();
    updateJump();
    updateMovementState();
    updateAnimation();
    applyCamera();
}

void PlayerController::updateCamera()
{
    // Camera input handling
    float dx = 0.0f;
    float dy = 0.0f;

    // Mouse input (when left mouse button is held)
    if (Input::instance().IsMouseButtonHeld(MouseButton::Left))
    {
        dx = static_cast<float>(Input::instance().GetCurrMouseX() - Input::instance().GetPrevMouseX());
        dy = static_cast<float>(Input::instance().GetCurrMouseY() - Input::instance().GetPrevMouseY());
    }

    // Gamepad right stick input
    float gamepadX = Input::instance().GetGamepadAxisValue(GamepadAxes::RightThumbX, 0);
    float gamepadY = Input::instance().GetGamepadAxisValue(GamepadAxes::RightThumbY, 0);

    const float gamepadSensitivity = 8.0f;
    dx += gamepadX * gamepadSensitivity;
    dy += -gamepadY * gamepadSensitivity;

    glm::vec3 playerPos = owner->getLocalPosition();

    if (perspective == Perspective::FIRST_PERSON)
    {
        // First person - free camera rotation
        firstPersonYaw += dx * cameraSensitivity;    // Horizontal rotation
        firstPersonPitch -= dy * cameraSensitivity;  // Vertical rotation

        // Clamp pitch to prevent flipping upside down
        firstPersonPitch = glm::clamp(firstPersonPitch, -89.0f, 89.0f);
    }
    else // THIRD_PERSON
    {
        // Third person - camera orbits independently of character
        cameraYaw += dx * cameraSensitivity;
        cameraPitch -= dy * cameraSensitivity;
       // cameraPitch = glm::clamp(cameraPitch, -89.0f, 89.0f);

        cameraPitch = glm::clamp(cameraPitch, 0.0f, 45.0f);
        //cameraYaw = glm::clamp(cameraYaw, -45.0f, 45.0f);
    }
}

glm::vec3 PlayerController::calculateMoveDirection()
{
    glm::vec3 direction(0.0f);

    // Compute camera horizontal forward vector (ignore pitch)
    float activeYaw = (perspective == Perspective::FIRST_PERSON) ? firstPersonYaw : cameraYaw;

    float yawRad = glm::radians(activeYaw);
    glm::vec3 forward = glm::normalize(glm::vec3(sin(yawRad), 0.0f, cos(yawRad)));
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    float dirMultiplier = (perspective == Perspective::FIRST_PERSON) ? -1.0f : 1.0f;

    // Keyboard input
    if (Input::instance().IsKeyHeld(KeyCode::W))
        direction -= forward * dirMultiplier;
    if (Input::instance().IsKeyHeld(KeyCode::S))
        direction += forward * dirMultiplier;
    if (Input::instance().IsKeyHeld(KeyCode::A))
        direction += right * dirMultiplier;
    if (Input::instance().IsKeyHeld(KeyCode::D))
        direction -= right * dirMultiplier;


    // Gamepad left stick input
    float gamepadX = Input::instance().GetGamepadAxisValue(GamepadAxes::LeftThumbX, 0);
    float gamepadY = Input::instance().GetGamepadAxisValue(GamepadAxes::LeftThumbY, 0);

    // Apply circular deadzone to avoid stick drift
    const float deadzone = 0.2f;
    float stickMagnitude = sqrt(gamepadX * gamepadX + gamepadY * gamepadY);

    if (stickMagnitude > deadzone)
    {
        // Normalize and apply deadzone scaling
        float normalizedX = gamepadX / stickMagnitude;
        float normalizedY = gamepadY / stickMagnitude;
        float adjustedMagnitude = (stickMagnitude - deadzone) / (1.0f - deadzone);

        // Scale back to get smooth analog movement
        gamepadX = -normalizedX * adjustedMagnitude;
        gamepadY = -normalizedY * adjustedMagnitude;

        // Convert to camera-relative movement
        direction += forward * gamepadY;
        direction += right * gamepadX;
    }

    // Normalize direction
    if (glm::length(direction) > 0.0f)
        direction = glm::normalize(direction);

    return direction;
}

void PlayerController::updateMovement()
{
    float dt = static_cast<float>(FramerateController::instance().GetFrameTime());

    // Calculate move direction based on camera
    moveDirection = calculateMoveDirection();

    // Check if running (Shift key OR Left Trigger on gamepad)
    isRunning = Input::instance().IsKeyHeld(KeyCode::LCtrl) ||
        Input::instance().IsGamepadButtonHeld(GamepadButton::LEFT_THUMB, 0);

    // Calculate speed
    float speed = moveSpeed;
    if (isRunning && glm::length(moveDirection) > 0.0f)
        speed *= runSpeedMultiplier;

    // Apply movement

    if (physicsComp)
    {
        glm::vec3 currentVel = physicsComp->getVelocity();
        isGrounded = abs(currentVel.y) < 0.5f;

        if (glm::length(moveDirection) > 0.0f)
        {
            // MOVING - Set target velocity
            glm::vec3 targetVelocity = moveDirection * speed;
            currentVel.x = targetVelocity.x;
            currentVel.z = targetVelocity.z;
            // Keep Y velocity for gravity

            physicsComp->setVelocity(currentVel);


            if (perspective == Perspective::FIRST_PERSON)
            {
                // First person: player rotates with camera
                owner->setLocalRotation(glm::vec3(0.0f, glm::radians(firstPersonYaw), 0.0f));
            }
            // Rotate player to face movement direction
            if (glm::length(glm::vec2(moveDirection.x, moveDirection.z)) > 0.0f)
            {
                float targetYaw = atan2(moveDirection.x, moveDirection.z);
                glm::vec3 eulerRotation = glm::vec3(0.0f, targetYaw, 0.0f);
                glm::quat quatRotation = glm::quat(eulerRotation);

                owner->setLocalRotation(eulerRotation);

                // CRITICAL: Sync rotation to physics body so it persists!
                physicsComp->setRotation(quatRotation);
            }

        }
        else
        {
            // Zero out horizontal velocity completely
            physicsComp->setVelocity(glm::vec3(0.0f, currentVel.y, 0.0f));

            if (perspective == Perspective::FIRST_PERSON)
            {
                owner->setLocalRotation(glm::vec3(0.0f, glm::radians(firstPersonYaw), 0.0f));
            }
        }
        //std::cout << "Player velocity: " << currentVel.x << "," << currentVel.z << std::endl;
    }
    else
    {
        if (glm::length(moveDirection) > 0.0f)
        {
            glm::vec3 velocity = moveDirection * speed;
            // Direct transform movement
            glm::vec3 currentPos = owner->getLocalPosition();
            owner->setLocalPosition(currentPos + velocity * dt);
            currentmovement = velocity * dt;
            std::cout << "Player moved to: " << owner->getLocalPosition().x << ","<< owner->getLocalPosition().z << std::endl;
            if (glm::length(glm::vec2(moveDirection.x, moveDirection.z)) > 0.0f)
            {
                // Rotate player to face movement direction
                float targetYaw = atan2(moveDirection.x, moveDirection.z);
                owner->setLocalRotation(glm::vec3(0.0f, targetYaw, 0.0f));
            }
        }
    }
}

void PlayerController::updateJump()
{
    // Check ground status (simplified - you should use proper physics collision)
    if (physicsComp)
    {
        glm::vec3 velocity = physicsComp->getVelocity();
        isGrounded = abs(velocity.y) < 0.1f;
    }

    // Jump input (Space key OR A button on gamepad)
    bool jumpPressed = Input::instance().IsKeyPressed(KeyCode::Space) ||
        Input::instance().IsGamepadButtonPressed(GamepadButton::A, 0);

    if(jumpPressed)
    {
        cout << "Jump pressed. isGrounded: " << isGrounded << endl;
    }

    if (jumpPressed && isGrounded)
    {
        if (physicsComp)
        {
            // Apply upward impulse instead of setting velocity
            glm::vec3 impulse(0.0f, jumpForce, 0.0f);
            physicsComp->addImpulse(impulse);
        }

        currentState = MovementState::JUMPING;
    }
}

void PlayerController::updateMovementState()
{
    if (!isGrounded)
    {
        currentState = MovementState::JUMPING;
        /*if (physicsComp && physicsComp->getVelocity().y < 0.0f)
            currentState = MovementState::FALLING;*/
        /*if (physicsComp && physicsComp->getVelocity().y < 0.0f)
            currentState = MovementState::JUMPING;
        else if (currentState != MovementState::JUMPING)
            currentState = MovementState::IDLE;*/
        return;
    }

    // Ground movement states
    if (glm::length(moveDirection) < 0.01f)
    {
        currentState = MovementState::IDLE;
    }
    else if (isRunning)
    {
        currentState = MovementState::RUNNING;
    }
    else
    {
        currentState = MovementState::WALKING;
    }
}

void PlayerController::updateAnimation()
{
    if (!skeletalMesh)
        return;

    // Play appropriate animation based on state
    if (previousState != currentState)
    switch (currentState)
    {
    case MovementState::IDLE:
       // std::cout << "Idle anim\n";
        skeletalMesh->playAnimClip(idleAnimName, true);
        break;
    case MovementState::WALKING:
      //  std::cout << "walking anim\n";
        skeletalMesh->playAnimClip(walkAnimName, true);
        break;
    case MovementState::RUNNING:
      //  std::cout << "runiing anim\n";
        skeletalMesh->playAnimClip(runAnimName, true);
        break;
    case MovementState::JUMPING:
     //   std::cout << "Jump anim\n";
        skeletalMesh->playAnimClip(jumpAnimName, false);
        break;
    case MovementState::FALLING:
     //   std::cout << "fall anim\n";
        //skeletalMesh->playAnimClip(jumpAnimName, false); //change to falling animation later
        break;
    }
    previousState = currentState;
    //skeletalMesh->update();
}

void PlayerController::applyCamera()
{
    glm::vec3 playerPos = owner->getLocalPosition();

    if (perspective == Perspective::FIRST_PERSON)
    {

        // Camera at player's eye level
        glm::vec3 eyeOffset(0.0f, 5.0f, 2.0f);  // Eye height
        glm::vec3 cameraPosition = playerPos + eyeOffset;

        cam.eye = cameraPosition;
        cam.tilt = firstPersonPitch;
        cam.spin = firstPersonYaw;
        cam.update();
    }
    else // THIRD_PERSON
    {
        // Third person - camera behind player
        cam.setTarget(playerPos);
        cam.setThirdPersonAngle(cameraYaw, cameraPitch);
        cam.setthirdPersonDistance(cameraDistance);
        cam.update();
    }
}


////////////////////////////
/// Lua binding
void PlayerController::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent(
        "PlayerController",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {
            Perspective p = Perspective::THIRD_PERSON;
            if (args.size() > 0)
            {
                std::string perspStr = args[0];
                if (perspStr == "first_person")
                    p = Perspective::FIRST_PERSON;
            }
            return std::make_shared<PlayerController>(owner, p);
        },
        0,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<PlayerController>(comp);
            return sol::make_object(L, derived);
        }
    );
    bindToLua(lua);
}

void PlayerController::bindToLua(sol::state& lua)
{
    lua.new_usertype<PlayerController>("PlayerController",
        sol::base_classes, sol::bases<Component>(),
        "setMoveSpeed", &PlayerController::setMoveSpeed,
        "setRunSpeedMultiplier", &PlayerController::setRunSpeedMultiplier,
        "setJumpForce", &PlayerController::setJumpForce,
        "setCameraDistance", &PlayerController::setCameraDistance,
        "setCameraSensitivity", &PlayerController::setCameraSensitivity,
        "setPerspective", &PlayerController::setPerspective,
        "setAnimationNames", &PlayerController::setAnimationNames,
        "getActionPressed", &PlayerController::getActionPressed
    );
}
