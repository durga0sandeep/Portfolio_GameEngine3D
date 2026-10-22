/*!****************************************************************************
 * file  : GBuffer.h
 * brief : GBuffer - This class handles the creation and management of a G-Buffer,
 * including multiple textures for position, diffuse color, normal,
 * and texture coordinates. It also manages the frame buffer object (FBO)
 * and depth texture for deferred rendering.
 *
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once


 /**
  * @class GBuffer
  * @brief Implements a deferred shading G-Buffer for storing scene geometry information.
  *
  * The GBuffer class manages multiple render targets (textures) to store
  * per-pixel information such as positions, normals, diffuse color, and
  * texture coordinates. It is primarily used in deferred shading pipelines.
  *
  * @note After binding the G-Buffer with `bindFBO()`, render geometry to populate
  *       the textures. Use `bindTexture()` to access them in shaders for lighting.
  */
class GBuffer
{
public:

    /**
     * @enum GBUFFER_TEXTURE_TYPE
     * @brief Types of textures stored in the G-Buffer.
     */
    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,   /**< Stores world-space positions. */
        GBUFFER_TEXTURE_TYPE_DIFFUSE,    /**< Stores diffuse color. */
        GBUFFER_TEXTURE_TYPE_NORMAL,     /**< Stores normals. */
        GBUFFER_TEXTURE_TYPE_TEXCOORD,   /**< Stores texture coordinates. */
        GBUFFER_NUM_TEXTURES             /**< Total number of G-Buffer textures. */
    };

    /**
     * @brief Default constructor.
     */
    GBuffer();

    /**
     * @brief Destructor.
     */
    ~GBuffer();

    bool init(unsigned int WindowWidth, unsigned int WindowHeight);

    void bindFBO();

    void unbindFBO();

    void bindTexture(const int unit, const int programId, const std::string& name, const int textureId);

    void unbindTexture(const int unit);

    /** @brief Framebuffer object handle. */
    unsigned int fbo;

    /** @brief Array of G-Buffer textures. */
    unsigned int textures[GBUFFER_NUM_TEXTURES];

    /** @brief Depth texture handle. */
    unsigned int depth_texture;

private:
    // No private members yet
};
