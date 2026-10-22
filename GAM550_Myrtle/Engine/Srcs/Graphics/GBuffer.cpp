/*
#include <glbinding/gl/gl.h>
#pragma warning(disable: 4251)`
#include <glbinding/Binding.h>
#pragma warning(disable: 4251)
using namespace gl;
*/


#include "pch.h"
#include "Graphics/gbuffer.h"

#define ARRAY_SIZE_IN_ELEMENTS(m) sizeof(m)/sizeof(m[0])

GBuffer::GBuffer()
{
}

GBuffer::~GBuffer()
{
}


/**
 * @brief Initializes the G-Buffer with the specified dimensions.
 * @param WindowWidth Width of the rendering window.
 * @param WindowHeight Height of the rendering window.
 * @return true if initialization succeeds, false otherwise.
 */
bool GBuffer::init(unsigned int WindowWidth, unsigned int WindowHeight)
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned int depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
        WindowWidth, WindowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, depthBuffer);


    for (int i = 0; i < 4; ++i)
    {
        glGenTextures(1, &textures[i]);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA32F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)((int)GL_COLOR_ATTACHMENT0 + i),
            GL_TEXTURE_2D, textures[i], 0);
    }
    // Check for completeness/correctness
    int status = (int)glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != int(GL_FRAMEBUFFER_COMPLETE))
        printf("FBO Error: %d\n", status);

    // Unbind the fbo until it's ready to be used
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;

}


/**
 * @brief Binds the G-Buffer for rendering.
 */
void GBuffer::bindFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}


/**
 * @brief Unbinds the G-Buffer, returning to the default framebuffer.
 */
void GBuffer::unbindFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/**
 * @brief Binds a specific G-Buffer texture to a texture unit for shader use.
 * @param unit Texture unit index.
 * @param programId Shader program ID.
 * @param name Name of the uniform in the shader.
 * @param textureId Index of the texture to bind.
 */
void GBuffer::bindTexture(const int unit, const int programId, const std::string& name, const int textureId)
{
    
    glActiveTexture((GLenum)((int)GL_TEXTURE0 + unit));
    glBindTexture(GL_TEXTURE_2D, textures[textureId]);
    int loc = glGetUniformLocation(programId, name.c_str());
    glUniform1i(loc, unit);
}

/**
 * @brief Unbinds a texture from the specified texture unit.
 * @param unit Texture unit index.
 */
void GBuffer::unbindTexture(const int unit)
{
    glActiveTexture((GLenum)((int)GL_TEXTURE0 + unit));
    glBindTexture(GL_TEXTURE_2D, 0);
}

