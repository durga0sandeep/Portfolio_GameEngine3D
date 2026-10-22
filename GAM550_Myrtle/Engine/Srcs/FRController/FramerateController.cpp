/*!****************************************************************************
 * file  : FramerateController.cpp
 * brief : 
 * Framework for controlling framerate and timing in a game application.
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#include "pch.h"
#include "FRController/FramerateController.h"
//#include "glfw3.h"

#define MILLISECOND 1000

/**
 * $BRIEF
 * constructor
 */
FramerateController::FramerateController()
{
    frameCount = 0;
    timeAtSpecificFrame = 0.0;

    // example physics
    accumulator = 0.0;
    SetTargetFramerate(60);
}

/**
 * $BRIEF
 * destructor
 */
void FramerateController::SetTargetFramerate(unsigned int framerate)
{
    targetFrameTime = (int)(MILLISECOND / framerate);
}

/**
 * $BRIEF
 * set the frame number at the start of a level
 * \param time
 */
void FramerateController::SetFrameAtLevelStart(const int time)
{
    frameAtLevelStart = time;
}

/**
 * $BRIEF
 * mark the start of a frame
 */
void FramerateController::FrameStart()
{
    timeAtFrameStart = glfwGetTime();
}

/**
 * $BRIEF
 * mark the end of a frame, sleep if necessary to maintain target framerate
 */
void FramerateController::FrameEnd()
{
    lastRenderTime = glfwGetTime() - timeAtFrameStart;

    timeAtFrameEnd = glfwGetTime();
    double delta = timeAtFrameEnd - timeAtFrameStart;
    double tt = (targetFrameTime / 1000.0);
    while (timeAtFrameEnd - timeAtFrameStart < tt)
        timeAtFrameEnd = glfwGetTime();
    //double frameDuration = glfwGetTime() - timeAtFrameStart;
    //double targetDuration = targetFrameTime / 1000.0;
    //double sleepTime = targetDuration - frameDuration;

    //if (sleepTime > 0.0)
    //    std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));


    ++frameCount;

    lastFrameTime = timeAtFrameEnd - timeAtFrameStart;
    DeltaTime = (float)lastFrameTime;

    // example physics
    accumulator += DeltaTime;
}

/**
 * $BRIEF
 * mark the time at a specific frame number
 * \param frameNumber
 */
void FramerateController::MarkTimeAtFrame(int frameNumber)
{
    if (frameCount == frameNumber)
        timeAtSpecificFrame = glfwGetTime();
}

/**
 * $BRIEF
 * get the time at the last marked frame
 * \return \b 
 */
float FramerateController::GetTimeAtFrameLastMarkedFrame()
{
    return float(timeAtSpecificFrame);
}

/**
 * $BRIEF
 * get the elapsed time since the last marked frame
 * \return \b 
 */
double FramerateController::GetElapsedTimeSinceLastMarkedFrame()
{
    return glfwGetTime() - timeAtSpecificFrame;
}

/**
 * $BRIEF
 * get the time taken to render the last frame
 * \return \b 
 */
double FramerateController::GetFrameTime() const
{
    return lastFrameTime;
}

/**
 * $BRIEF
 * get the current frames per second
 * \return \b 
 */
float FramerateController::GetFPS() const
{
    return 1.0f / (float)lastFrameTime;
}

/**
 * $BRIEF
 * get the current frame number
 * \return \b 
 */
int FramerateController::GetFrameNumber() const
{
    return frameCount;
}

/**
 * $BRIEF
 * reset the frame number to zero
 */
void FramerateController::ResetFrameNummber()
{
    frameCount = 0;
}

/**
 * $BRIEF
 * get the time taken to render the last frame
 * \return \b 
 */
double FramerateController::GetRenderTime() const
{
    return lastRenderTime;
}

/**
 * $BRIEF
 * get the frame number at the start of a level
 * \return \b 
 */
double FramerateController::GetFrameAtLevelStart() const
{
    return frameAtLevelStart;
}

/**
 * $BRIEF
 * get the target frame time in milliseconds
 * \return \b 
 */
double FramerateController::GetTime() const
{
    return targetFrameTime;
}

/**
 * $BRIEF
 * get the current time in seconds since the application started
 * \return \b 
 */
double FramerateController::GetSeconds() const
{
    return glfwGetTime();
}

/**
 * $BRIEF
 * cheack if it's time to update physics
 * \return \b 
 */
bool FramerateController::ShouldUpdatePhysics()
{
    return accumulator >= DEFAULT_FIXED_TIME_STEP;
}

/**
 * $BRIEF
 * consume the time used for physics update
 */
void FramerateController::ConsumePhysicsTime()
{
    accumulator -= DEFAULT_FIXED_TIME_STEP;
}

/**
 * $BRIEF
 * initialize the framerate controller
 */
void FramerateController::initialize()
{
}

/**
 * $BRIEF
 * shutdown the framerate controller
 */
void FramerateController::shutdown()
{
}
