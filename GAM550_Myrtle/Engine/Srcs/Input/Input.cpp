#include "pch.h"
#include "Input/Input.h"
#include <Windows.h>
#include <GLFW/glfw3native.h>

/**
 * $BRIEF
 * distructor, ensure input thread is stop
 */
Input::~Input()
{
    //make sure thread is exit
    running = false;       // Signal thread to stop
    if (inputThread.joinable()) {
        inputThread.join(); // Wait for thread to finish
        std::cout << "input background thread release";
    }
}

/**
 * $BRIEF
 * Initializes the input system with a reference to the game window.
 * This sets up the internal pointer to the window and prepares gamepad input.
 * \param window
 */
void Input::initialize(GameWindow* window)
{
	pWindow = window;
    windowWidth.store(static_cast<float>(window->getWidth()));
    windowHeight.store(static_cast<float>(window->getHeight()));
	InitGamepads();
}

/**
 * $BRIEF
 * initializes button states, axes, and connection status for each gamepad using XInput.
 */
void Input::InitGamepads()
{
    const std::vector<GamepadButton> allButtons = {
        GamepadButton::DPAD_UP, GamepadButton::DPAD_DOWN,
        GamepadButton::DPAD_LEFT, GamepadButton::DPAD_RIGHT,
        GamepadButton::START, GamepadButton::BACK,
        GamepadButton::LEFT_THUMB, GamepadButton::RIGHT_THUMB,
        GamepadButton::LEFT_SHOULDER, GamepadButton::RIGHT_SHOULDER,
        GamepadButton::A, GamepadButton::B,
        GamepadButton::X, GamepadButton::Y
    };

    for (int gamepadId = 0; gamepadId < MAX_GAMEPADS; ++gamepadId) // XInput supports up to 4 controllers
    {
        auto& writeButtons = (*writebufferGamepadButtons)[gamepadId];
        auto& readButtons = (*readbufferGamepadButtons)[gamepadId];
        auto& writeAxes = (*writebuffergamepadAxes)[gamepadId];
        auto& readAxes = (*readbuffergamepadAxes)[gamepadId];

        // Initialize button states
        for (GamepadButton button : allButtons)
        {
            int btnValue = static_cast<int>(button);
            writeButtons.try_emplace(btnValue, ButtonState());
            readButtons.try_emplace(btnValue, ButtonState());
        }

        // Initialize axes (XInput has 6: 4 thumbstick axes + 2 triggers)
        for (int axis = 0; axis < 6; ++axis)
        {
            writeAxes.try_emplace(axis, 0.0f);
            readAxes.try_emplace(axis, 0.0f);
        }

        // Check if controller is connected
        XINPUT_STATE state = {};
        DWORD result = XInputGetState(gamepadId, &state);
        (*writegamepadConnected)[gamepadId] = (result == ERROR_SUCCESS);
        (*readgamepadConnected)[gamepadId] = (result == ERROR_SUCCESS);
    }
}

/**
 * $BRIEF
 * background thread, thread safe
 */
void Input::startInputThread()
{
    running.store(true);
    inputThread = std::thread([this]() {
        while (running) {
            updateWrite(); // Accesses member function
            std::this_thread::yield();
        }
        });
}

//test only
void Input::stopInputThread()
{
    running = false;       // Signal thread to stop
    if (inputThread.joinable()) {
        inputThread.join(); // Wait for thread to finish
        std::cout << "input background thread release";
    }
}

/**
 * $BRIEF
 * BackGround Update function call
 */
void Input::updateWrite()
{
    if (swap.load())
    {
        // We just got a new write buffer after the swap
        // Reset THIS buffer (the new write buffer) FIRST
        ResetTransientStates();

        // NOW poll fresh input into the clean buffer
        UpdateKeyStates();
        UpdateMouseStates();
        UpdateGamepadStates();
        UpdateMousePostion();

        // Signal we're done writing
        swap.store(false);
    }
}

/**
 * $BRIEF
 * This is the update call from main thread, should swap the read and write buffer every time
 */
void Input::update()
{

    CopyHeldStates();

   auto* currentWriteKey = writeBufferKeyBoard.load();
   readBufferKeyBoard.store(currentWriteKey);
   writeBufferKeyBoard.store(currentWriteKey == &keyStatesA ? &keyStatesB : &keyStatesA);

   // Mouse Buttons
   auto* currentWriteMouse = writeMouseButtonStates.load();
   readMouseButtonStates.store(currentWriteMouse);
   writeMouseButtonStates.store(currentWriteMouse == &mouseButtonStatesA ? &mouseButtonStatesB : &mouseButtonStatesA);

   // Gamepad Buttons
   auto* currentWriteGamepadButtons = writebufferGamepadButtons.load();
   readbufferGamepadButtons.store(currentWriteGamepadButtons);
   writebufferGamepadButtons.store(currentWriteGamepadButtons == &gamepadButtonsA ? &gamepadButtonsB : &gamepadButtonsA);

   // Gamepad Axes
   auto* currentWriteGamepadAxes = writebuffergamepadAxes.load();
   readbuffergamepadAxes.store(currentWriteGamepadAxes);
   writebuffergamepadAxes.store(currentWriteGamepadAxes == &gamepadAxesA ? &gamepadAxesB : &gamepadAxesA);

   // Gamepad Connection Status
   auto* currentWriteGamepadConnected = writegamepadConnected.load();
   readgamepadConnected.store(currentWriteGamepadConnected);
   writegamepadConnected.store(currentWriteGamepadConnected == &gamepadConnectedA ? &gamepadConnectedB : &gamepadConnectedA);
   swap.store(true);
    
}


/**
 * $BRIEF
 * Update Key State, no need to reset
 */
void Input::UpdateKeyStates()
{
	for (auto& [key, state] : *writeBufferKeyBoard) {
        // this is not need for back ground thread
		////reset the states for the new frame
		/*state.pressed = false;
		state.released = false;*/

        int winState = GetAsyncKeyState(key);

        if (winState & 0x8000)
        { // Key is down
            if (!state.held)
            {
                state.pressed = true;
            }
            state.held = true;
        }
        else
        {
            if (state.held)
            {
                state.released = true;
            }
            state.held = false;
        }
	}
}

void Input::UpdateMouseStates()
{
	for (auto& [key, state] : *writeMouseButtonStates) {

		//get the current key state
		int glfwState = GetAsyncKeyState(key);

		if ((glfwState & 0x8000) != 0)
        {
			if (!state.held) {
				state.pressed = true;
			}
			state.held = true;
		}
		else
        {
			if (state.held) {
				state.released = true;
			}
			state.held = false;
		}
	}

    double currentx = currentMouseY.load();
    double currenty = currentMouseY.load();
	GetMousePosition(currentx, currenty);
    currentMouseX.store(currentx);
    currentMouseY.store(currenty);
}

void Input::UpdateGamepadStates()
{
    for (int gamepadId = 0; gamepadId < XUSER_MAX_COUNT; ++gamepadId) {
        XINPUT_STATE state = {};
        DWORD result = XInputGetState(gamepadId, &state);

        (*writegamepadConnected)[gamepadId] = (result == ERROR_SUCCESS);

        if (result == ERROR_SUCCESS) {
            // Handle buttons
            WORD buttons = state.Gamepad.wButtons;
            auto& buttonsState = (*writebufferGamepadButtons)[gamepadId];

            for (auto& [button, btnState] : buttonsState) {
                btnState.pressed = false;
                btnState.released = false;

                bool isPressed = (buttons & button) != 0;

                if (isPressed) {
                    if (!btnState.held) {
                        btnState.pressed = true;
                    }
                    btnState.held = true;
                }
                else {
                    if (btnState.held) {
                        btnState.released = true;
                    }
                    btnState.held = false;
                }
            }

            // Handle axes
            auto& axesState = (*writebuffergamepadAxes)[gamepadId];

            // Normalize thumbsticks [-32768, 32767] → [-1.0, 1.0]
            axesState[0] = state.Gamepad.sThumbLX / 32767.0f;
            axesState[1] = state.Gamepad.sThumbLY / 32767.0f;
            axesState[2] = state.Gamepad.sThumbRX / 32767.0f;
            axesState[3] = state.Gamepad.sThumbRY / 32767.0f;

            // Normalize triggers [0, 255] → [0.0, 1.0]
            axesState[4] = state.Gamepad.bLeftTrigger / 255.0f;
            axesState[5] = state.Gamepad.bRightTrigger / 255.0f;
        }
    }
}

void Input::CopyHeldStates()
{
    // Determine which buffer will become the new write buffer after swap
    auto* currentWriteKey = writeBufferKeyBoard.load();
    auto* futureWriteKey = (currentWriteKey == &keyStatesA) ? &keyStatesB : &keyStatesA;

    auto* currentWriteMouse = writeMouseButtonStates.load();
    auto* futureWriteMouse = (currentWriteMouse == &mouseButtonStatesA) ? &mouseButtonStatesB : &mouseButtonStatesA;

    auto* currentWriteGamepadButtons = writebufferGamepadButtons.load();
    auto* futureWriteGamepadButtons = (currentWriteGamepadButtons == &gamepadButtonsA) ? &gamepadButtonsB : &gamepadButtonsA;

    // Copy keyboard held states
    for (auto& [key, writeState] : *currentWriteKey) {
        (*futureWriteKey)[key].held = writeState.held;
    }

    // Copy mouse button held states
    for (auto& [button, writeState] : *currentWriteMouse) {
        (*futureWriteMouse)[button].held = writeState.held;
    }

    // Copy gamepad button held states for all gamepads
    for (int gamepadId = 0; gamepadId < MAX_GAMEPADS; ++gamepadId) {
        auto& currentGamepadButtons = (*currentWriteGamepadButtons)[gamepadId];
        auto& futureGamepadButtons = (*futureWriteGamepadButtons)[gamepadId];

        for (auto& [button, writeState] : currentGamepadButtons) {
            futureGamepadButtons[button].held = writeState.held;
        }
    }
}

bool Input::IsKeyPressed(KeyCode key)
{
	int glfwKey = static_cast<int>(key);
	return (*readBufferKeyBoard)[glfwKey].pressed;
}

bool Input::IsKeyReleased(KeyCode key)
{
	int glfwKey = static_cast<int>(key);
	return (*readBufferKeyBoard)[glfwKey].released;
}

bool Input::IsKeyHeld(KeyCode key)
{
	int glfwKey = static_cast<int>(key);
	return (*readBufferKeyBoard)[glfwKey].held;
}

bool Input::IsMouseButtonPressed(MouseButton button)
{
	int glfwMouseButton = static_cast<int>(button);
    return (*readMouseButtonStates)[glfwMouseButton].pressed;
}

bool Input::IsMouseButtonReleased(MouseButton button)
{
	int glfwMouseButton = static_cast<int>(button);
	return !(*readMouseButtonStates)[glfwMouseButton].released;
}

bool Input::IsMouseButtonHeld(MouseButton button)
{
	int glfwMouseButton = static_cast<int>(button);
	return (*readMouseButtonStates)[glfwMouseButton].held;
}

void Input::GetMousePosition(double& x, double& y)
{
	// Stores the position in x and y
    POINT cursorPos;
    double mouseX;
    double mouseY;
    if (GetCursorPos(&cursorPos)) {
        //glm::vec2 result = GetMouseInGlfw(cursorPos);
        //mouseX = result.x;
        //mouseY = result.y;
        mouseX = cursorPos.x;
        mouseY = cursorPos.y;
    }
    else
    {
        return;
    }
	
    //   float w = windowWidth.load();
    //   float h = windowHeight.load();
    //   //glfwGetCursorPos(pWindow->GetNativeWindow(), &mouseX, &mouseY);

	//x = (2 * mouseX / w) - 1;
	//y = (2 * mouseY / h) - 1;
    x = mouseX;
    y = mouseY;
}


glm::vec2 Input::GetMouseInGlfw(POINT input) const
{
    // Step 2: Convert screen coordinates to window client coordinates
    HWND hwnd = glfwGetWin32Window(pWindow->getNativeWindow());
    ScreenToClient(hwnd, &input);

    // Step 3: Return as glm::vec2 or any other format you prefer
    return glm::vec2(static_cast<float>(input.x), static_cast<float>(input.y));
}

void Input::bindToLua(sol::state& lua)
{
    lua["Input"] = std::ref(Input::instance());
}

void Input::SetCursorMode(CursorMode mode)
{
	glfwSetInputMode(pWindow->getNativeWindow(), GLFW_CURSOR, static_cast<int>(mode));
}

/**
 * $BRIEF
 * reset all button every keyframem, in the background loop
 */
void Input::ResetTransientStates()
{
    for (auto& [key, state] : *writeBufferKeyBoard) {
        state.pressed = false;
        state.released = false;
    }
    for (auto& [key, state] : *writeMouseButtonStates) {
        state.pressed = false;
        state.released = false;
    }
    for(int i=0; i< MAX_GAMEPADS;i++)
    {
        for (auto& [key, state] : (*writebufferGamepadButtons)[i]) {
            state.pressed = false;
            state.released = false;
        }
    }
}

/**
 * $BRIEF
 * update mouse position, during swap
 */
void Input::UpdateMousePostion()
{
    prevMouseX.store(read_currentMouseX.load());
    prevMouseY.store(read_currentMouseY.load());
    read_currentMouseX.store(currentMouseX.load());
    read_currentMouseY.store(currentMouseY.load());
}

/**
 * $BRIEF
 * get button press
 * \param button
 * \param playerId
 * \return \b 
 */
bool Input::IsGamepadButtonPressed(GamepadButton button, int playerId)
{
	int glfwGamepadButton = static_cast<int>(button);
	return GetGamepadButtonState(glfwGamepadButton, playerId, ButtonStateChecker::Pressed);
}

/**
 * $BRIEF
 * get button Release
 * \param button
 * \param playerId
 * \return \b 
 */
bool Input::IsGamepadButtonReleased(GamepadButton button, int playerId)
{
	int glfwGamepadButton = static_cast<int>(button);
	return GetGamepadButtonState(glfwGamepadButton, playerId, ButtonStateChecker::Released);
}

/**
 * $BRIEF
 * get button held
 * \param button
 * \param playerId
 * \return \b 
 */
bool Input::IsGamepadButtonHeld(GamepadButton button, int playerId)
{
	int glfwGamepadButton = static_cast<int>(button);
	return GetGamepadButtonState(glfwGamepadButton, playerId, ButtonStateChecker::Held);
}

/**
 * $BRIEF
 * get axis value, with joystick id
 * \param axisButton
 * \param playerId
 * \return \b 
 */
float Input::GetGamepadAxisValue(GamepadAxes axisButton, int playerId)
{
	int glfwGamepadAxis = static_cast<int>(axisButton);

	int gamepadId = GLFW_JOYSTICK_1 + playerId;

    auto connected = readgamepadConnected.load();

	if (connected->at(gamepadId))
	{
		//get the gamepad's axes state
		auto& axesState = (*readbuffergamepadAxes)[gamepadId];

		auto it = axesState.find(glfwGamepadAxis);
		if (it != axesState.end())
		{
			return it->second; 
		}
	}

	return 0.0f;
}

/**
 * $BRIEF
 * return active gamepad count
 * \return \b 
 */
int Input::GetConnectedGamepadCount()
{
    auto connected = readgamepadConnected.load();
    int count = 0;
    for (bool isConnected : *connected) {
        if (isConnected) {
            ++count;
        }
    }
    return count;
}

/**
 * $BRIEF
 * game pad connected check
 * \param controllerId
 * \return \b 
 */
bool Input::IsGamepadConnected(int controllerId)
{
    if (controllerId < 0 || controllerId >= MAX_GAMEPADS) {
        return false; // or throw an error if you prefer
    }
    auto connected = readgamepadConnected.load();
    return (*connected)[controllerId];
}
//
//glm::vec2 Input::GetMouseInGlfw(POINT input) const
//{
//    // Step 2: Convert screen coordinates to window client coordinates
//    HWND hwnd = glfwGetWin32Window(pWindow->getNativeWindow());
//    ScreenToClient(hwnd, &input);
//
//    // Step 3: Return as glm::vec2 or any other format you prefer
//    return glm::vec2(static_cast<float>(input.x), static_cast<float>(input.y));
//}

/**
 * $BRIEF
 * GetGamepadButtonState
 * \param button
 * \param playerId
 * \param state
 * \return \b 
 */
bool Input::GetGamepadButtonState(int button, int playerId, ButtonStateChecker state)
{
	int gamepadId = GLFW_JOYSTICK_1 + playerId;

    auto connected = readgamepadConnected.load();

	if (connected->at(gamepadId))
	{
		//get std::unorderedmap gamepad's button states from the array
		auto& buttonsState = (*readbufferGamepadButtons)[gamepadId];

		//check if the button exists
		auto it = buttonsState.find(button);
		if (it != buttonsState.end()) {
			const ButtonState& keyState = it->second;

			switch (state)
			{
			case ButtonStateChecker::Released:
				return keyState.released;
			case ButtonStateChecker::Held:
				return keyState.held;
			case ButtonStateChecker::Pressed:
				return keyState.pressed;
			default:
				return false;
			}
		}
	}

	//no controller or button found
	return false;
}

//void Input::JoystickCallback(int jid, int event) {
//	
//	Input& input = Input::Instance();
//
//	if (event == GLFW_CONNECTED) {
//		std::cout << "Joystick " << jid << " connected\n";
//	}
//	else if (event == GLFW_DISCONNECTED) {
//		std::cout << "Joystick " << jid << " disconnected\n";
//	}
//}
