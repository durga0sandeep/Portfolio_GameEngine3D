/*!****************************************************************************
 * file  : InputCodes
 * brief : 
 * Maps GLWF input codes to different Enum values
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")  // Link the library manually

/**
* @brief Enum mapping the different GLFW key codes. These are 1:1 with GLWF key codes: https://www.glfw.org/docs/3.3/group__keys.html
*/
enum class KeyCode
{
    Unknown = -1,
    Space = VK_SPACE,
    Escape = VK_ESCAPE,
    Enter = VK_RETURN,
    Tab = VK_TAB,
    Backspace = VK_BACK,
    LCtrl = VK_LCONTROL,
    Left = VK_LEFT,
    Right = VK_RIGHT,
    Up = VK_UP,
    Down = VK_DOWN,
    A = 0x41, // 'A'
    B = 0x42, // 'B'
    C = 0x43, // 'C'
    D = 0x44, // 'D'
    E = 0x45, // 'E'
    F = 0x46, // 'F'
    G = 0x47, // 'G'
    H = 0x48, // 'H'
    I = 0x49, // 'I'
    J = 0x4A, // 'J'
    K = 0x4B, // 'K'
    L = 0x4C, // 'L'
    M = 0x4D, // 'M'
    N = 0x4E, // 'N'
    O = 0x4F, // 'O'
    P = 0x50, // 'P'
    Q = 0x51, // 'Q'
    R = 0x52, // 'R'
    S = 0x53, // 'S'
    T = 0x54, // 'T'
    U = 0x55, // 'U'
    V = 0x56, // 'V'
    W = 0x57, // 'W'
    X = 0x58, // 'X'
    Y = 0x59, // 'Y'
    Z = 0x5A, // 'Z'
    F11 = VK_F11,
    Tilda = VK_OEM_3, // '~' / '`'
    Last = VK_OEM_3  // Use highest defined key if needed
};


/**
* @brief Enum mapping the different GLFW mouse buttons. These are 1:1 with GLWF mouse buttons: https://www.glfw.org/docs/latest/group__buttons.html
*/
enum class MouseButton
{
    Left    = VK_LBUTTON,
    Right   = VK_RBUTTON,
    Middle  = VK_MBUTTON,
    Button4 = VK_XBUTTON1,
    Button5 = VK_XBUTTON2
};



/*!****************************************************************************
 * maaping from inputX
 *
 *****************************************************************************/
enum class GamepadButton : int {
    DPAD_UP = 0x0001,  // XINPUT_GAMEPAD_DPAD_UP
    DPAD_DOWN = 0x0002,  // XINPUT_GAMEPAD_DPAD_DOWN
    DPAD_LEFT = 0x0004,  // XINPUT_GAMEPAD_DPAD_LEFT
    DPAD_RIGHT = 0x0008,  // XINPUT_GAMEPAD_DPAD_RIGHT
    START = 0x0010,  // XINPUT_GAMEPAD_START
    BACK = 0x0020,  // XINPUT_GAMEPAD_BACK
    LEFT_THUMB = 0x0040,  // XINPUT_GAMEPAD_LEFT_THUMB
    RIGHT_THUMB = 0x0080,  // XINPUT_GAMEPAD_RIGHT_THUMB
    LEFT_SHOULDER = 0x0100,  // XINPUT_GAMEPAD_LEFT_SHOULDER
    RIGHT_SHOULDER = 0x0200,  // XINPUT_GAMEPAD_RIGHT_SHOULDER
    A = 0x1000,  // XINPUT_GAMEPAD_A
    B = 0x2000,  // XINPUT_GAMEPAD_B
    X = 0x4000,  // XINPUT_GAMEPAD_X
    Y = 0x8000,  // XINPUT_GAMEPAD_Y
    Last_Button = Y
};



enum class GamepadAxes
{
    LeftThumbX = 0, // XINPUT_GAMEPAD.sThumbLX
    LeftThumbY = 1, // XINPUT_GAMEPAD.sThumbLY
    RightThumbX = 2, // XINPUT_GAMEPAD.sThumbRX
    RightThumbY = 3, // XINPUT_GAMEPAD.sThumbRY
    LeftTrigger = 4, // XINPUT_GAMEPAD.bLeftTrigger
    RightTrigger = 5  // XINPUT_GAMEPAD.bRightTrigger
};

