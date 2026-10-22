/*!****************************************************************************
 * file  : GameWindow.h
 * brief : GameWindow - Encapsulates GLFW window creation and management for OpenGL rendering.
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>
#include <functional>
#include <iostream>

/**
 * @class GameWindow
 * @brief A wrapper class that manages a GLFW window for OpenGL-based applications.
 *
 * The GameWindow class encapsulates the creation, management, and destruction
 * of a GLFW window. It provides utility methods for handling window events,
 * querying window dimensions, and setting custom callbacks such as resize handlers.
 *
 * This class simplifies GLFW usage by managing window lifecycle, initialization,
 * and user-defined configuration through functional callbacks.
 *
 * @note The class must be initialized with valid GLFW context settings before use.
 *       It also handles cleanup automatically upon destruction.
 *
 * @see GLFWwindow
 */
class GameWindow {
public:

    GameWindow(const std::string& title);

    GameWindow(int width, int height, const std::string& title);

    ~GameWindow();

    bool shouldClose() const;

    void pollEvents();

    GLFWwindow* getNativeWindow() const;

    void setWindowHints(const std::function<void()>& hintSetter);

    int getWidth() const;
    int getHeight() const;

    void setResizeCallback(std::function<void(GLFWwindow*, int, int)> callback);

private:
    void initialize();

    void shutdown();

    static void resizeCallbackWrapper(GLFWwindow* window, int width, int height);

    int width;
    int height;
    std::string title;
    GLFWwindow* pWindow;
    std::function<void(GLFWwindow*, int, int)> resizeCallback;

};
