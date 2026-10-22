/*!****************************************************************************
 * file  : UIButton.h
 * brief : UIButton - The UIButton class represents a clickable button in the UI.
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <Graphics/Mesh.h>
#include <Graphics/Material.h>
#include <sol/sol.hpp>

class UIButton {
public:
    enum class ButtonState {
        Normal,
        Focused,
        Pressed
    };

    //UIButton(glm::vec2 pos, glm::vec2 sz, std::shared_ptr<Texture> tex, std::function<void()> callback);
    // use deafault img
    UIButton(glm::vec2 pos, const std::string& text, sol::function callback);
    UIButton(glm::vec2 pos, glm::vec2 sz,
        std::shared_ptr<Texture> normalTex,
        std::shared_ptr<Texture> focusedTex,
        std::shared_ptr<Texture> pressedTex,
        sol::function callback);

    glm::vec2 position;
    glm::vec2 size;
    sol::function onClick;
    
    std::shared_ptr<Material> buttonMaterial;
    std::shared_ptr<Texture> texture;

    static void initializeButtonResources();
    static void initializeBtnImage(const std::string& normalImgPath,
        const std::string& focusedImgPath,
        const std::string& clickedImgPath);

    void setCustomTextures(std::shared_ptr<Texture> normalTex,
        std::shared_ptr<Texture> focusedTex,
        std::shared_ptr<Texture> pressedTex) {
        customNormalTexture = normalTex;
        customFocusedTexture = focusedTex;
        customPressedTexture = pressedTex;
        useDeaultImg = false; 
    }

    // State management
    void setFocused(bool focused);
    void setPressed(bool pressed);
    bool isFocused() const { return btnState == ButtonState::Focused || btnState == ButtonState::Pressed; }

    void setEnabled(bool enabled) { focusEnabled = enabled; }
    bool isEnabled() const { return focusEnabled; }

    void setPosition(const glm::vec2& pos) { position = pos; }
    void setSize(const glm::vec2& sz) { size = sz; }
    glm::vec2 getPosition() const { return position; }
    glm::vec2 getSize() const { return size; }

    // when Space is pressed
    void activate();

    bool isPointInside(float x, float y) const;


    /**
     * $BRIEF
     * check if the mouse is hovering over the button, need to implement
     * \param mouseX
     * \param mouseY
     * \return \b 
     */
    bool isHovered(float mouseX, float mouseY) const {
        return mouseX >= position.x && mouseX <= position.x + size.x &&
            mouseY >= position.y && mouseY <= position.y + size.y;
    }

    /**
     * $BRIEF
     * still need to implement
     * \param mouseX
     * \param mouseY
     */
    void tryClick(float mouseX, float mouseY) {
        if (isHovered(mouseX, mouseY) && onClick) {
            onClick();
        }
    }

    void draw() const;

    void setActive(bool in_active);

    bool isActive()
    {
        return active;
    }
   

private:
    std::shared_ptr<class UIText> btnText;
    bool active = true;

    ButtonState btnState = ButtonState::Normal;
    bool focusEnabled =  false;
    bool useDeaultImg = false;

    static std::shared_ptr<Texture> defaultNormalTexture;
    static std::shared_ptr<Texture> defaultFocusedTexture;
    static std::shared_ptr<Texture> defaultPessedTexture;

    std::shared_ptr<Texture> customNormalTexture;
    std::shared_ptr<Texture> customFocusedTexture;
    std::shared_ptr<Texture> customPressedTexture;

    static std::shared_ptr<Shader> buttonShader;
    static std::shared_ptr<Mesh> buttonMesh;

    std::shared_ptr<Texture> getCurrentTexture() const;
};
