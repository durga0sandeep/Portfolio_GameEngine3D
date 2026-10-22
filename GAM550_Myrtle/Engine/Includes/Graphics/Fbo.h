#pragma once
#include "pch.h"

class FBO {
public:

    unsigned int fboID;
    unsigned int textureID, textureID2, textureID3, textureID4;
    std::vector<GLuint> textureIDs;
    int width, height;  // Size of the texture.

    //SSBO
    unsigned int ssboID;
    size_t indicesSize;

    FBO(const int w, const int h);
    void createFBO(const int w, const int h);
    void createFboMulti(const int w, const int h);

    // Bind this FBO to receive the output of the graphics pipeline.
    void bindFBO();

    // Unbind this FBO from the graphics pipeline;  graphics goes to screen by default.
    void unbindFBO();

    // Bind this FBO's texture to a texture unit.
    void bindTexture(const int unit, const int programId, const std::string& name);
    void bindTexture4(const int unit, const int programId, const std::string& name);

    // Unbind this FBO's texture from a texture unit.
    void unbindTexture(const int unit);

    int getFboDimension();



    void bindImageTextureCS(const int unit, const GLenum access, const GLenum format, int ShaderId, const char* name);


    /*
    void LoadTextureIDs();
    void CreateSSBO(size_t lightsSize);
    void PopulateSSBOFromTextures();//cannot say need to use it or not
    */
};



