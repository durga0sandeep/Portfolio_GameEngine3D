/*!****************************************************************************
 * file  : input.h
 * brief : 
 * Authors: Mariojulio Alvarez - base code, Alex Humphries - upgrades and refactors,
 * Ju-ve Chankasemporn - worked with controller and InputActions
 * Martin Chow upgrade to window.h and Xinput and background input system
 *  Description: Handles input for engine.
 *  Uses windows and Xinput callbacks in order to get inputs on demand
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include <unordered_map>
#include "InputCodes.h"
#include <array>
#include <thread>
#include <atomic>
#include <Graphics/GameWindow.h>
#include <Xinput.h>
#include "LuaManager/LuaManager.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h> // Needed for glfwGetWin32Window
#pragma comment(lib, "Xinput.lib")  // Link the library manually

// TODO - Possible optimization is making this dynamic
// based on how many controllers connected
#define MAX_GAMEPADS 4
/**
* @brief Enum describing the different cursor "mode{s}". These are 1:1 with GLWF cursor modes. Ref: https://www.glfw.org/docs/3.3/group__input.html
*/
enum class CursorMode
{
	Normal, // Cursor is visible as normal
	Hidden, // hides the cursor in the content area (window) but doesn't lock it
	Disabled // hides and grabs (locks) the cursor
};

//use for checking the current state of the buttons only
enum class ButtonStateChecker {
	Pressed,
	Released,
	Held
};

struct ButtonState {
	bool pressed = false;   //true - only on the frame the key is first pressed
	bool released = false;  //true - only on the frame the key is released
	bool held = false;      //true - while the key is being held
};

class Input
{
public:
	static Input& instance() {
		static Input instance;
		return instance;
	}
	
	void initialize(GameWindow*);
	// Called by game loop
    void startInputThread();
    //test only
    void stopInputThread();
	void update();
    

	/* Keyboard */
	bool IsKeyPressed(KeyCode key);
	bool IsKeyReleased(KeyCode key);
	bool IsKeyHeld(KeyCode key);

	/* Mouse */
	bool IsMouseButtonPressed(MouseButton button);
	bool IsMouseButtonReleased(MouseButton button);
	bool IsMouseButtonHeld(MouseButton button);

	void GetMousePosition(double& x, double& y);
	double GetCurrMouseX() const { return read_currentMouseX.load(); }
	double GetCurrMouseY() const { return read_currentMouseY.load(); }
	double GetPrevMouseX() const { return prevMouseX.load(); }
	double GetPrevMouseY() const { return prevMouseY.load(); }
	void SetCursorMode(CursorMode mode);

    void ResetTransientStates();
    void UpdateMousePostion();

	/* Gamepad */
	bool IsGamepadButtonPressed(GamepadButton button, int controllerId = 0);
	bool IsGamepadButtonReleased(GamepadButton button, int controllerId = 0);
	bool IsGamepadButtonHeld(GamepadButton button, int controllerId = 0);
	float GetGamepadAxisValue(GamepadAxes axisButton, int controllerId = 0);
    int GetConnectedGamepadCount();
    bool IsGamepadConnected(int controllerId);
    glm::vec2 GetMouseInGlfw(POINT input) const;
    void bindToLua(sol::state& lua);

private:
	Input() = default;
	~Input();

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

    void updateWrite();
	void UpdateKeyStates();
	void UpdateMouseStates();
	void InitGamepads();
	void UpdateGamepadStates();
	void CopyHeldStates();
	bool GetGamepadButtonState(int button, int controllerId, ButtonStateChecker state);

    // this is only use for set curser
    // please dont touch pwindow with background thread, it is not thread safe!
    GameWindow* pWindow{ nullptr };

    // create 2 buffer
    std::unordered_map<int, ButtonState> keyStatesA;// Handles keyboard key states
    std::unordered_map<int, ButtonState> mouseButtonStatesA; // Handles mouse button states (left click, etc.)
    std::array<std::unordered_map<int, ButtonState>, MAX_GAMEPADS> gamepadButtonsA;
    std::array<std::unordered_map<int, float>, MAX_GAMEPADS> gamepadAxesA;
    std::array<bool, MAX_GAMEPADS> gamepadConnectedA = { false };

    std::unordered_map<int, ButtonState> keyStatesB; // Handles keyboard key states
	std::unordered_map<int, ButtonState> mouseButtonStatesB; // Handles mouse button states (left click, etc.)
	std::array<std::unordered_map<int, ButtonState>, MAX_GAMEPADS> gamepadButtonsB;
	std::array<std::unordered_map<int, float>, MAX_GAMEPADS> gamepadAxesB;
    std::array<bool, MAX_GAMEPADS> gamepadConnectedB = {false};

    // for swap chain pointer of the read and write buffer
    std::atomic<std::unordered_map<int, ButtonState>*> readBufferKeyBoard = &keyStatesA;
    std::atomic<std::unordered_map<int, ButtonState>*> writeBufferKeyBoard = &keyStatesB;
    std::atomic<std::unordered_map<int, ButtonState>*> readMouseButtonStates = &mouseButtonStatesA;
    std::atomic<std::unordered_map<int, ButtonState>*> writeMouseButtonStates = &mouseButtonStatesB;
    std::atomic<std::array<std::unordered_map<int, ButtonState>, MAX_GAMEPADS>*> readbufferGamepadButtons = &gamepadButtonsA;
    std::atomic<std::array<std::unordered_map<int, ButtonState>, MAX_GAMEPADS>*> writebufferGamepadButtons = &gamepadButtonsB;
    std::atomic<std::array<std::unordered_map<int, float>, MAX_GAMEPADS>*> readbuffergamepadAxes = &gamepadAxesA;
    std::atomic<std::array<std::unordered_map<int, float>, MAX_GAMEPADS>*> writebuffergamepadAxes = &gamepadAxesB;
    std::atomic<std::array<bool, MAX_GAMEPADS>*> readgamepadConnected = &gamepadConnectedA;
    std::atomic<std::array<bool, MAX_GAMEPADS>*> writegamepadConnected = &gamepadConnectedB;

    std::atomic<double> currentMouseX{0.0f};
    std::atomic<double> currentMouseY{0.0f};
    std::atomic<double> read_currentMouseX{ 0.0f };
    std::atomic<double> read_currentMouseY{ 0.0f };
    std::atomic<double> prevMouseX{0.0f};
    std::atomic<double> prevMouseY{0.0f};

    std::atomic<float> windowWidth;
    std::atomic<float> windowHeight;

    //mutithreading
    std::thread inputThread;
    std::atomic<bool> running = false;
    std::atomic<bool> swap = true;

	//static void JoystickCallback(int jid, int event);
};
