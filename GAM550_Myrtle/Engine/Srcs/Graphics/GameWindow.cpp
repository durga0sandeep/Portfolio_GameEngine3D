#include "pch.h"
#include "Graphics/GameWindow.h"
#include <stdexcept>
#include <iostream>



/**
 * @brief Constructs a GameWindow object and initializes a GLFW window.
 *
 * @param width The width of the window in pixels.
 * @param height The height of the window in pixels.
 * @param title The title of the window displayed in the title bar.
 *
 * @throws std::runtime_error if GLFW initialization or window creation fails.
 */
GameWindow::GameWindow(int width, int height, const std::string& title)
    : width(width), height(height), title(title), pWindow(nullptr) {
    initialize();
}



/**
 * @brief Constructs a GameWindow using the primary monitor's screen resolution.
 *
 * This constructor initializes GLFW (if not already initialized), retrieves the
 * primary monitor's video mode, and automatically sets the window width and height
 * based on the monitor's current resolution. It then calls initialize() to finish
 * setting up the window.
 *
 * @param title The title to display on the window.
 *
 * @throws std::runtime_error If GLFW fails to initialize.
 */
GameWindow::GameWindow(const std::string& title)
    : width(0), height(0), title(title), pWindow(nullptr)
{
    // Initialize GLFW first (if not already done)
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Get primary monitor
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Use monitor resolution
    width = mode->width;
    height = mode->height;

    initialize();
}


/**
 * @brief Destructor that shuts down and cleans up GLFW resources.
 */
GameWindow::~GameWindow() {
    shutdown();
}

/**
 * @brief Checks whether the user has requested to close the window.
 *
 * @return true if the window should close, false otherwise.
 */
bool GameWindow::shouldClose() const {
    return glfwWindowShouldClose(pWindow);
}

/**
 * @brief Polls for and processes pending window events.
 *
 * This function should be called once per frame in the main loop
 * to ensure input and window updates are handled properly.
 */
void GameWindow::pollEvents() {
    glfwPollEvents();
}

/**
 * @brief Returns the native GLFWwindow pointer.
 *
 * @return A pointer to the underlying GLFWwindow object.
 */
GLFWwindow* GameWindow::getNativeWindow() const {
    return pWindow;
}

/**
 * @brief Sets custom window hints before window creation.
 *
 * @param hintSetter A callable function that sets GLFW window hints.
 *
 * @note This should be called before initializing the window.
 */
void GameWindow::setWindowHints(const std::function<void()>& hintSetter) {
    hintSetter();
}

/**
 * @brief Gets the current width of the window.
 *
 * @return The window width in pixels.
 */
int GameWindow::getWidth() const {
    return width;
}

/**
 * @brief Gets the current height of the window.
 *
 * @return The window height in pixels.
 */
int GameWindow::getHeight() const {
    return height;
}

/**
 * @brief Registers a callback function that triggers when the window is resized.
 *
 * @param callback A function to be called whenever the framebuffer size changes.
 */
void GameWindow::setResizeCallback(std::function<void(GLFWwindow*, int, int)> callback) {
    resizeCallback = std::move(callback);
    glfwSetFramebufferSizeCallback(pWindow, resizeCallbackWrapper);
    glfwSetWindowUserPointer(pWindow, this);
}

/**
 * @brief Initializes GLFW and creates a window.
 *
 * @throws std::runtime_error if GLFW initialization or window creation fails.
 */
void GameWindow::initialize() {

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwSwapInterval(1); // Enable V-Sync

    if (!pWindow) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }



    //
    
}

/**
 * @brief Destroys the window and terminates GLFW.
 */
void GameWindow::shutdown() {
    if (pWindow) {
        glfwDestroyWindow(pWindow);
    }
    glfwTerminate();
}

/**
 * @brief Static wrapper function that forwards resize events to the user-defined callback.
 *
 * @param window Pointer to the GLFWwindow that received the event.
 * @param width The new width of the window in pixels.
 * @param height The new height of the window in pixels.
 *
 * @note This function retrieves the GameWindow instance associated with the GLFWwindow
 *       and calls its resize callback if one is set.
 */
void GameWindow::resizeCallbackWrapper(GLFWwindow* window, int width, int height) {
    auto* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    if (gameWindow && gameWindow->resizeCallback) {
        gameWindow->resizeCallback(window, width, height);
        std::cout << "Window size: (" << width << ", " << height << ")" << std::endl;
    }
}
