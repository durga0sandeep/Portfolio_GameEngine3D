#include "pch.h"
#include "UI/UIButton.h"
#include "UI/UIText.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine.h>

std::shared_ptr<Shader> UIButton::buttonShader = nullptr;
std::shared_ptr<Mesh> UIButton::buttonMesh = nullptr;
std::shared_ptr<Texture> UIButton::defaultNormalTexture = nullptr;
std::shared_ptr<Texture> UIButton::defaultFocusedTexture = nullptr;
std::shared_ptr<Texture> UIButton::defaultPessedTexture = nullptr;

/**
*  Debug only
 * @brief Checks for OpenGL errors and logs them with the specified location.
 * @param location Description of where the check is performed.
 */
void checkGLError(const char* location) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error at " << location << ": " << err << std::endl;
    }
}

//UIButton::UIButton(glm::vec2 pos, glm::vec2 sz, std::shared_ptr<Texture> tex, std::function<void()> callback)
//    : position(pos), size(sz), onClick(callback), texture(tex)
//{
//    buttonMaterial = std::make_shared<Material>(UIButton::buttonShader);
//}

UIButton::UIButton(glm::vec2 pos, const std::string& text, sol::function callback)
    : position(pos), size(glm::vec2(200.0f, 60.0f)), onClick(callback)
    , btnState(ButtonState::Normal), focusEnabled(true), useDeaultImg(true)
{
    buttonMaterial = std::make_shared<Material>(UIButton::buttonShader);
    size = glm::vec2(200.0f, 60.0f);
    
    btnText = std::make_shared<UIText>(pos+glm::vec2(20,10), text, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

UIButton::UIButton(glm::vec2 pos, glm::vec2 sz,
    std::shared_ptr<Texture> normalTex,
    std::shared_ptr<Texture> focusedTex,
    std::shared_ptr<Texture> pressedTex,
    sol::function callback)
    : position(pos), onClick(callback), btnState(ButtonState::Normal),
    focusEnabled(true), useDeaultImg(false),
    customNormalTexture(normalTex),
    customFocusedTexture(focusedTex),
    customPressedTexture(pressedTex) {

    buttonMaterial = std::make_shared<Material>(UIButton::buttonShader);

    size = sz;

    // No text for icon buttons
    btnText = nullptr;
}

/**
 * $BRIEF
 * create a simple quad mesh for button rendering
 */
void UIButton::initializeButtonResources()
{
    const std::string shader_path = "Shader/button_vertex_shader.vertex\nShader/button_fragment_shader.fragment";

    UIButton::buttonShader = std::make_shared<Shader>(shader_path);

    UIButton::buttonMesh = std::make_shared<Mesh>("UIButton");

    std::vector<float> vertices = {
        // x, y,    
        0.0f, 0.0f,  // bottom-left
        1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f,  // top-right
        0.0f, 1.0f   // top-left
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };
    buttonMesh->setVertexData(
        {
            {GeometryBuffer::Attribute::Position, vertices, 2},
        },
        2 * sizeof(float), // stride: 2 pos + 2 texCoord
        indices
        );
}

void UIButton::initializeBtnImage(const std::string& normalImgPath,
    const std::string& focusedImgPath,
    const std::string& clickedImgPath) {

    defaultNormalTexture = Texture::create(normalImgPath);
    defaultFocusedTexture = Texture::create(focusedImgPath);
    defaultPessedTexture = Texture::create(clickedImgPath);

    std::cout << "Normal texture path: " << normalImgPath << std::endl;
}


// btn's collision detect
bool UIButton::isPointInside(float x, float y) const
{

    return x >= position.x && x <= position.x + size.x &&
        y >= position.y && y <= position.y + size.y;
}


void UIButton::setFocused(bool focused) {
    if (!focusEnabled) return;

    if (focused) {
        btnState = ButtonState::Focused;
    }
    else {
        btnState = ButtonState::Normal;
    }
}

void UIButton::setPressed(bool pressed) {
    if (!focusEnabled) return;

    if (pressed) {
        btnState = ButtonState::Pressed;
    }
    else {
        btnState = ButtonState::Focused; // Return to focused after press
    }
}

void UIButton::activate() {
    if (!focusEnabled) return;

    if (onClick) {
        onClick();
    }
}

/**
 * $BRIEF
 * draw the button on screen
 */
void UIButton::draw() const
{
    checkGLError("After Button draw");
    //glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    buttonMaterial->setProperty("position", position);
    buttonMaterial->setProperty("size", size);
    buttonMaterial->setProperty("screenSize", Engine::instance().getWindowSize());
    //buttonMaterial->setProperty("buttonColor", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    
    
    std::shared_ptr<Texture> currentTexture;

    if (useDeaultImg) {
        switch (btnState) {
        case ButtonState::Normal:
            currentTexture = defaultNormalTexture;
            break;
        case ButtonState::Focused:
            currentTexture = defaultFocusedTexture;
            break;
        case ButtonState::Pressed:
            currentTexture = defaultPessedTexture;
            break;
        }
    }
    else {
        switch (btnState) {
        case ButtonState::Normal:
            currentTexture = customNormalTexture;
            break;
        case ButtonState::Focused:
            currentTexture = customFocusedTexture;
            break;
        case ButtonState::Pressed:
            currentTexture = customPressedTexture;
            break;
        }
    }

    if (!currentTexture) {
        currentTexture = texture;
    }
     
    // TODO: add texture support
    //buttonMaterial->setProperty("useTexture", 0);
    //buttonMaterial->setProperty("projection", projection);
    //buttonMaterial->setTexture("ButtonTexture", texture,0);
    buttonMaterial->setTexture("ButtonTexture", currentTexture, 0);
    buttonMaterial->apply();

    buttonMesh->getGeometryBuffer()->bind();

    
    if (buttonMesh->getGeometryBuffer()->getIndexCount() > 0) {
        glDrawElements(GL_TRIANGLES, buttonMesh->getGeometryBuffer()->getIndexCount(), GL_UNSIGNED_INT, 0);
    }
    checkGLError("After Button draw");

    if (btnText) {
        btnText->draw();
    }

    buttonMesh->getGeometryBuffer()->unbind();
}

void UIButton::setActive(bool in_active)
{
    active = in_active;
    if(btnText)
    btnText->setActive(in_active);
};
