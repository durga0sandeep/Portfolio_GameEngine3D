///*!****************************************************************************
// * file  : UIComponent.h
// * brief : UI Component - Bridge between GameObject system and UI Manager
// *
// * author: Yen-Ling Chang
// * Course: GAM550
// * date  : November 2025
// * Copyright © 2025 DigiPen Institute of Technology
// /****************************************************************************/
//
//#pragma once
//#include <string>
//#include <vector>
//#include <functional>
//#include "Graphics/Renderer.h"
//#include "Components/Component.h"
//#include "UI/UICanvas.h"
//#include "Entities/GameObject.h"
//
//typename enum UIType
//{
//    PANEL, //takes up entire window size
//    TEXT,
//    BUTTON //interactable
//};
//
///*
//ct uiElement
//{
//    std::string image_path;
//    std::string 
//    Type        type;
//    float       height;
//    float       width;
//    float[][]   position_on_screen;
//
//    uiElement(std::string path, type, height, width, position_x, position_y)
//    {
//        
//    }
//
//    setPos();
//    setSize();
//};
//  3 classes:
//
//  1) button
//  2) panel (Cover the entire background)
//  3) text
//
//  things in common:
//    they all get added to a canvas
//    they all have size/position
//    you can assign an image to any of those
//
//
//
//    first create a 
//*/
//
//class UIComponent : public Component
//{
//public:
//    //UIComponent(GameObject* _owner, const std::string& texturePath,glm::vec2 pos,glm::vec2 size,int num);
//    //UIComponent(GameObject* _owner, const std::string& texturePath, glm::vec2 pos, glm::vec2 size, UIType type);
//    UIComponent(GameObject* _owner, const std::string _name,const std::string &image_path,UIType type, float x, float y, float w, float h);
//
//    ~UIComponent();
//
//   /* std::shared_ptr<UICanvas> getCanvas() { return canvas; }
//    void setActive(bool active);
//    bool isActive() const { return isCanvasActive; }*/
//
//    //void useDefaultTextures();
//
//    // button Callback registration (called from Lua)
//    //void setOnClick(sol::function callback);
//
//    // For text customization (only for Text type)
//    void setTextColor(float r, float g, float b);
//    void setTextScale(float scale);
//    void setSize(float x, float y);
//    void setLuaOnClick(sol::function func);
//    void onClick();
//    void setPos(float x, float y);
//    void setIsActive(bool active);
//    bool getIsActive();
//
//    // Build the actual UI element and add to UIManager
//    //void build();
//
//    // Component lifecycle
//    //void update(float deltaTime) override {}
//    //void setLuaStart(sol::function func);
//    //id update() override { }
//
//    static void bindToLua(sol::state& lua);
//    static void registerWithFactory(sol::state& lua);
//    //static void registerWithFactory();
//
//private:
//    //std::string name;
//    std::string texturePath;
//    std::string name;
//    glm::vec2 size;
//    glm::vec2 pos;
//    UIType type;
//    bool built = false;
//    bool useDefaultImg = false;
//    std::string data;
//
//    float textScale = 1.0f;
//    glm::vec3 textColor{ 1.0f, 1.0f, 1.0f };
//
//    bool isActive;
//    sol::function luaOnClick;
//    //sol::function luaCallback;
//
//    //std::shared_ptr<UIButton> button;
//    //std::shared_ptr<UIText> uiText;
//    //std::shared_ptr<UIPanel> panel;
//
//    //std::shared_ptr<UICanvas> canvas;
//    //std::string canvasName;
//    //bool isCanvasActive;
//};
//
///*
//
//canvas1 = gameobject:addcomponent(ui component, , , )
//canvas1 : addButton(button name, image or non if none it takes the default, pos, size, etc.., name of functionality).....
//{
//    //add it to the correct place and the correct functionality
//
//    //add it to the buttons unorderd map in the uicanvas
//}
//
//
//
//canvas1:addPanel()....
//
//
//*/
//
//
