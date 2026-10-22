/*!*******************************************************************
 * file   Engine.cpp
 * brief  : This file contains the implementation of the Engine class
 *
 * author: Salam Sibai , Martin Chow
 * Course:  GAM550
 * date:   September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 *********************************************************************/

#include "pch.h"
#include "Engine.h"
// this should not be here, should be in Sence manager or something
// place holder for now
#include <UI/UIManager.h>
#include "Components/ComponentList.h"
// TODO: change to singleton pattern
// should be Renderer::instance()
// AssetLoadManager should rename instance()
/**
 * $BRIEF
 * constructor for the Engine class
 */
Engine::Engine() : renderer(Renderer::instance()), physics_manager(PhysicsManager::instance()),
                   asset_manager(AssetLoadManager::instance()), framerate_controller(FramerateController::instance()),
                   window(std::make_unique<GameWindow>("Test")),
                   input_manager(Input::instance()), audio_manager(AudioManager::instance()), audio(audio_manager),
                   scene_manager(SceneManager::instance()), lua_manager(LuaManager::instance())
{
    renderer.initialize(window.get());
            
    renderer.initDeferredRendering();

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    lua_manager.initialize();
    audio_manager.init();
    audio_manager.loadSound("..\\assets\\audio\\temp_bgm.wav", "bgm");
    audio_manager.loadSoundSFX("..\\assets\\audio\\SFX_box\\footstep_carpet_000.ogg", "collide");
    // audio_manager.loadSound("../assets/audio_manager/temp_bgm.wav", "bgm");
    std::cout << std::filesystem::absolute("../assets/audio_manager/temp_bgm.wav") << std::endl;

    /*!****************************************************************************
     *                       Scene Creation
     ******************************************************************************/

    // render objects
    renderer.initializeRenderObjects();
    
    //deferredSetUp();//Sandeep

    physics_manager.setGravity(glm::vec3(0, -98.1f, 0));

    physics_manager.setDebugWire();
    physics_manager.setDebugVelocity();
    
    input_manager.initialize(window.get());
    audio_manager.playBGM("bgm");

    framerate_controller.SetTargetFramerate(120);

    

}

void Engine::runEngine()
{
    input_manager.startInputThread();
    lua_manager.update(0);
    while (!window->shouldClose())
    {
        framerate_controller.FrameStart();
        
        PhysicsManager::instance().update(static_cast<float>(framerate_controller.GetFrameTime()));
        ParticleSystemManager::instance().update(static_cast<float>(framerate_controller.GetFrameTime()));

        lua_manager.update(static_cast<float>(framerate_controller.GetFrameTime()));
        // scene_manager.runScene();
        // framerateText->setText("FPS: " + std::to_string((int)framerate_controller.GetFPS()));
        // should be in scene manager or something
        if (Input::instance().IsKeyPressed(KeyCode::K))
        {
            std::cout << "k press" << std::endl;
            audio_manager.playSFX("collide");
        }
        if (Input::instance().IsKeyReleased(KeyCode::B))
        {
            std::cout << "b press" << std::endl;
            renderer.setDebugMode(!renderer.getDebugMode());
        }

        //---------------------------------------------------------------------------
        /*
        if (Input::instance().IsKeyPressed(KeyCode::Q)) {
            //increase
            //renderer.msmMethods.incrementKernal();
        }

        if (Input::instance().IsKeyPressed(KeyCode::W)) {
            //decrease
            //renderer.msmMethods.decrementKernal();
        }
        */



        /*
        if (Input::instance().IsMouseButtonHeld(MouseButton::Left))
        {
            // std::cout <<  Input::instance().GetGamepadAxisValue(GamepadAxes::Axis_Left_X, 0) << std::endl;
        }
        if (Input::instance().IsGamepadButtonPressed(GamepadButton::LEFT_THUMB))
        {
            // std::cout << "Left Press" << std::endl;
        }
        */
        //---------------------------------------------------------------------------

        /*
        auto position = Bodies[1].getPosition();
        auto rotation = Bodies[1].getRotation();
        renderObjInfo2->getTransform()->setPosition(position);
        renderObjInfo2->getTransform()->setRotation(rotation);
        */

        if (Input::instance().IsMouseButtonPressed(MouseButton::Left))
        {
            double mouseX, mouseY;
            Input::instance().GetMousePosition(mouseX, mouseY);

            // Convert screen coordinates to window coordinates
            POINT screenPos = { static_cast<LONG>(mouseX), static_cast<LONG>(mouseY) };
            glm::vec2 windowPos = Input::instance().GetMouseInGlfw(screenPos);

            //// Flip Y coordinate (OpenGL has origin at bottom-left)
            //float uiY = static_cast<float>(window->getHeight()) - windowPos.y;  // Use your actual window height

            //std::cout << "Screen pos: (" << mouseX << ", " << mouseY << ")" << std::endl;
            //std::cout << "Window pos: (" << windowPos.x << ", " << windowPos.y << ")" << std::endl;
            //std::cout << "UI pos: (" << windowPos.x << ", " << uiY << ")" << std::endl;

            UIManager::instance().handleClick(windowPos.x, windowPos.y);
        }


        if (Input::instance().IsKeyPressed(KeyCode::Escape))
        {
            isUIVisible = !isUIVisible;

            // TODO: set different canvas
            /*if(isUIVisible)
                UIManager::instance().setActiveCanvas("TestCanvas");
            else
                UIManager::instance().setActiveCanvas("");*/

            //UIManager::instance().handleInput("ACTIVATE");
        }

        asset_manager.processGPUUploads();
        //  render part
        renderer.render(window->getNativeWindow());
        input_manager.update();
        audio_manager.update();
        framerate_controller.FrameEnd();
       
    }
}

glm::vec2 Engine::getWindowSize()
{
    return glm::vec2(window->getWidth(), window->getHeight());
}


Engine &Engine::instance()
{
    static Engine instance;
    return instance;
}
