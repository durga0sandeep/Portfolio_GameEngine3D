#pragma once
#include "pch.h"
#include"temp_old/object.h"
#include"temp_old/shapes.h"

class TempValues
{

private:

    float spin;
    float tilt;
    float tx;
    float ty;
    float zoom;
    float rx;
    float ry;
    float front;
    float back;
    float Rx, Ry;

    // Viewport
    int width, height;

    glm::mat4 WorldProj, WorldView, WorldInverse, L_ModelMatrix, L_ViewMatrix, L_ProjectionMatrix, ShadowMatrix, B;

    const float PI = 3.14159f;
    const float rad = PI / 180.0f;    // Convert degrees to radians

    glm::vec3 diffuseColor;          // Diffuse color of object
    glm::vec3 specularColor;         // Specular color of object
    float shininess;                // Surface roughness value

    // 
    // Light parameters
    float lightSpin, lightTilt, lightDist;
    glm::vec3 lightPos;
    glm::vec3 lightDir;
    //
    
    // World up direction (standard "up" is positive Y-axis)
    glm::vec3 worldUp;  // World "up" direction (Y-axis)
    // Compute the right direction (perpendicular to both lightDir and worldUp)
    glm::vec3 rightDir;  // Right direction
    // Compute the up direction (perpendicular to both lightDir and rightDir)
    glm::vec3 upDir;  // Up direction


    // @@ Perhaps declare additional scene lighting values here. (lightVal, lightAmb)
    glm::vec3 lightVal;// Brightness for micro-facet BRDF
    glm::vec3 lightAmb;//ambient light
    glm::vec3 lowSpecular;

    //   glm::vec3 Light, Ambient;
    glm::vec3 Light = glm::vec3(3.0, 3.0, 3.0);//vec3(0.1, 0.1, 0.1);//
    glm::vec3 Ambient = glm::vec3(0.2, 0.2, 0.2);//vec3(0.1, 0.1, 0.1);//vec3(0.2, 0.2, 0.2);

    float z_near;
    float z_far;

    int objectId;

    Object* objectRoot;
    Object* floor,* podium;

    Shapes* BoxPolygons;
    Shapes* FloorPolygons;


    Object* DS_Root;
    Shapes* ScreenQuadPolys;
    Object* ScreenQuad;

    //glm::mat4 Translate(const float x, const float y, const float z);

public:
    
    TempValues();
    void setup();
    glm::mat4 LookAt(const glm::vec3 Eye, const glm::vec3 Center, const glm::vec3 Up);
    glm::mat4 Perspective(const float rx, const float ry, const float front, const float back);
    glm::mat4 Translate(const float x, const float y, const float z);
    glm::mat4 Scale(const float x, const float y, const float z);
    glm::mat4 Rotate(const int i, const float theta);


    Object* getObjectRoot() {  return objectRoot; }
    Object* getQuadRoot() {  return DS_Root; }
    
    // ------------------------------
    // Getters
    // ------------------------------
    int getObjectId() const { return objectId; }

    float getSpin() const { return spin; }
    float getTilt() const { return tilt; }
    float getTx() const { return tx; }
    float getTy() const { return ty; }
    float getZoom() const { return zoom; }
    float getRx() const { return rx; }
    float getRy() const { return ry; }
    float getFront() const { return front; }
    float getBack() const { return back; }
    float getRxAngle() const { return Rx; }
    float getRyAngle() const { return Ry; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    float getLightSpin() const { return lightSpin; }
    float getLightTilt() const { return lightTilt; }
    float getLightDist() const { return lightDist; }

    glm::vec3 getLightPos() const { return lightPos; }
    glm::vec3 getLightDir() const { return lightDir; }
    glm::vec3 getWorldUp() const { return worldUp; }
    glm::vec3 getRightDir() const { return rightDir; }
    glm::vec3 getUpDir() const { return upDir; }

    glm::vec3 getLightVal() const { return lightVal; }
    glm::vec3 getLightAmb() const { return lightAmb; }
    glm::vec3 getLowSpecular() const { return lowSpecular; }
    glm::vec3 getLight() const { return Light; }
    glm::vec3 getAmbient() const { return Ambient; }

    float getZNear() const { return z_near; }
    float getZFar() const { return z_far; }

    glm::mat4 getWorldProj() const { return WorldProj; }
    glm::mat4 getWorldView() const { return WorldView; }
    glm::mat4 getWorldInverse() const { return WorldInverse; }
    glm::mat4 getLightModelMatrix() const { return L_ModelMatrix; }
    glm::mat4 getLightViewMatrix() const { return L_ViewMatrix; }
    glm::mat4 getLightProjMatrix() const { return L_ProjectionMatrix; }
    glm::mat4 getShadowMatrix() const { return ShadowMatrix; }
    glm::mat4 getBMatrix() const { return B; }

    glm::vec3 getDiffuseColor() { return diffuseColor; }
    glm::vec3 getSpecularColor() { return specularColor; }
    float getShininess() { return shininess; }


    // ------------------------------
    // Setters
    // ------------------------------
    void setSpin(float v) { spin = v; }
    void setTilt(float v) { tilt = v; }
    void setTx(float v) { tx = v; }
    void setTy(float v) { ty = v; }
    void setZoom(float v) { zoom = v; }
    void setRx(float v) { rx = v; }
    void setRy(float v) { ry = v; }
    void setFront(float v) { front = v; }
    void setBack(float v) { back = v; }
    void setRxAngle(float v) { Rx = v; }
    void setRyAngle(float v) { Ry = v; }

    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }

    void setLightSpin(float v) { lightSpin = v; }
    void setLightTilt(float v) { lightTilt = v; }
    void setLightDist(float v) { lightDist = v; }
    void setLightPos(const glm::vec3& v) { lightPos = v; }
    void setLightDir(const glm::vec3& v) { lightDir = glm::normalize(v); }
    void setWorldUp(const glm::vec3& v) { worldUp = v; }
    void setRightDir(const glm::vec3& v) { rightDir = v; }
    void setUpDir(const glm::vec3& v) { upDir = v; }

    void setLightVal(const glm::vec3& v) { lightVal = v; }
    void setLightAmb(const glm::vec3& v) { lightAmb = v; }
    void setLowSpecular(const glm::vec3& v) { lowSpecular = v; }
    void setLight(const glm::vec3& v) { Light = v; }
    void setAmbient(const glm::vec3& v) { Ambient = v; }

    void setZNear(float v) { z_near = v; }
    void setZFar(float v) { z_far = v; }

    void setWorldProj(const glm::mat4& m) { WorldProj = m; }
    void setWorldView(const glm::mat4& m) { WorldView = m; }
    void setWorldInverse(const glm::mat4& m) { WorldInverse = m; }
    void setLightModelMatrix(const glm::mat4& m) { L_ModelMatrix = m; }
    void setLightViewMatrix(const glm::mat4& m) { L_ViewMatrix = m; }
    void setLightProjMatrix(const glm::mat4& m) { L_ProjectionMatrix = m; }
    void setShadowMatrix(const glm::mat4& m) { ShadowMatrix = m; }
    void setBMatrix(const glm::mat4& m) { B = m; }


   
};

