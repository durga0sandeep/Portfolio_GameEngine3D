#include"pch.h"
#include "Graphics/Fbo.h"
#include <iostream>
///////////////////////////////////////////////////////////////////////
// A slight encapsulation of a Frame Buffer Object (i'e' Render
// Target) and its associated texture.  When the FBO is "Bound", the
// output of the graphics pipeline is captured into the texture.  When
// it is "Unbound", the texture is available for use as any normal
// texture.
////////////////////////////////////////////////////////////////////////

#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { fprintf(stderr, "OpenGL error (at line Fbo.cpp:%d): %s\n", __LINE__, gluErrorString(err)); exit(-1);} }


FBO::FBO(const int w, const int h) {

    std::cout << "THE TEXTURE ID init  "<< "Called first" << std::endl;

    width = w;
    height = h;

    textureID = 0, textureID2 = 0, textureID3 = 0, textureID4 = 0;

}


void FBO::createFBO(const int w, const int h)
{
    width = w;
    height = h;

    glGenFramebuffers(1, &fboID);//Gen to generate 
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    // Create a render buffer, and attach it to FBO's depth attachment
    unsigned int depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
        width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, depthBuffer);

    // Create a texture and attach FBO's color 0 attachment.  The
    // GL_RGBA32F and GL_RGBA constants set this texture to be 32 bit
    // floats for each of the 4 components.  Many other choices are
    // possible.
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, textureID, 0);

    // Check for completeness/correctness
    int status = (int)glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != int(GL_FRAMEBUFFER_COMPLETE))
        printf("FBO Error: %d\n", status);

    //glObjectLabel(GL_FRAMEBUFFER, fboID, -1, "DEBUG FBO SHADOW");
    //glObjectLabel(GL_TEXTURE, textureID, -1, "DEBUG FBO TEXTURE");

    //
    // Unbind the fbo until it's ready to be used
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //CHECKERROR;
}



void FBO::createFboMulti(const int w, const int h)
{
    width = w;
    height = h;

    glGenFramebuffers(1, &fboID);//Gen to generate 
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    // Create a render buffer, and attach it to FBO's depth attachment
    unsigned int depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
        width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, depthBuffer);

    // Create a texture and attach FBO's color 0 attachment.  The
    // GL_RGBA32F and GL_RGBA constants set this texture to be 32 bit
    // floats for each of the 4 components.  Many other choices are
    // possible.
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, textureID, 0);
    //
    glGenTextures(1, &textureID2);
    glBindTexture(GL_TEXTURE_2D, textureID2);
    glTexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
        GL_TEXTURE_2D, textureID2, 0);
    //
    glGenTextures(1, &textureID3);
    glBindTexture(GL_TEXTURE_2D, textureID3);
    glTexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
        GL_TEXTURE_2D, textureID3, 0);
    //
    glGenTextures(1, &textureID4);
    glBindTexture(GL_TEXTURE_2D, textureID4);
    glTexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
        GL_TEXTURE_2D, textureID4, 0);


    GLenum bufs[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, bufs);


    //
    // Check for completeness/correctness
    int status = (int)glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != int(GL_FRAMEBUFFER_COMPLETE))
        printf("FBO Error: %d\n", status);

    //
    // Unbind the fbo until it's ready to be used
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //CHECKERROR;
}


void FBO::bindFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    //CHECKERROR;
}

void FBO::unbindFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //CHECKERROR;
}

void FBO::bindTexture(const int unit, const int programId, const std::string& name)
{//

    //std::cout << "1 TEXTURE UNIT " << unit << std::endl;

    glActiveTexture((GLenum)((int)GL_TEXTURE0 + unit));
    //CHECKERROR;
    glBindTexture(GL_TEXTURE_2D, textureID);
    //CHECKERROR;
    int loc = glGetUniformLocation(programId, name.c_str());
    glUniform1i(loc, unit);
}


void FBO::bindTexture4(const int unit, const int programId, const std::string& name)
{//
    glActiveTexture((GLenum)((int)GL_TEXTURE0 + unit));
    //CHECKERROR;
    //glBindTexture(GL_TEXTURE_2D, textureID);

    //std::cout << "2 TEXTURE UNIT " << unit << std::endl;


    if (unit == 1) {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    else if (unit == 2) {
        glBindTexture(GL_TEXTURE_2D, textureID2);
    }
    else if (unit == 3) {
        glBindTexture(GL_TEXTURE_2D, textureID3);
    }
    else if (unit == 4) {
        glBindTexture(GL_TEXTURE_2D, textureID4);
    }

    //CHECKERROR;
    int loc = glGetUniformLocation(programId, name.c_str());
    glUniform1i(loc, unit);
}


void FBO::unbindTexture(const int unit)
{
    glActiveTexture((GLenum)((int)GL_TEXTURE0 + unit));
    glBindTexture(GL_TEXTURE_2D, 0);
    //CHECKERROR;

}

int FBO::getFboDimension( ) {

    return width;

}

void FBO::bindImageTextureCS(const int unit, const GLenum access, const GLenum format, int ShaderId,const char* name)
{
    glBindImageTexture(unit, textureID, 0, GL_FALSE, 0, access, format);
    int loc = glGetUniformLocation(ShaderId, name);
    glUniform1i(loc, unit);

}



/*
void FBO::CreateSSBO(size_t lightsSize, const PointLightData& light) {

    // Generate and bind SSBO
    glGenBuffers(1, &ssboID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboID);

    //size_t lightsSize = pointLights.size()
    indicesSize = textureIDs.size() * sizeof(int);

    // Allocate memory for the SSBO (e.g., to store 4 color attachment data)
    //glBufferData(GL_SHADER_STORAGE_BUFFER, 4 * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_SHADER_STORAGE_BUFFER, lightsSize * indicesSize, nullptr, GL_DYNAMIC_DRAW);


    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, lightsSize, light.data());
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, lightsSize, indicesSize, textureIDs.data());

    // Bind SSBO to binding point (binding point 0 in this example)
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboID);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // Unbind
    CHECKERROR;
}


void FBO::PopulateSSBOFromTextures() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboID);

    // Create a vector to store texture data
    std::vector<glm::vec4> textureData(4);

    // Bind each texture and read its data
    for (int i = 0; i < 4; ++i) {
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]); // Use your texture ID array
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &textureData[i]);
    }

    // Upload data to SSBO
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 4 * sizeof(glm::vec4), textureData.data());

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // Unbind SSBO
    CHECKERROR;
}


void FBO::LoadTextureIDs( ) {

    textureIDs.push_back(3);
    textureIDs.push_back(4);
    textureIDs.push_back(5);
    textureIDs.push_back(6);

}

*/


