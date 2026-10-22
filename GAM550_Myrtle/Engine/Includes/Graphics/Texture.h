/*!****************************************************************************
 * file  : Texture.h
 * brief : Texture - This class supports loading 2D, 3D, and cube map textures from files or
 * OpenGL texture IDs. It provides functions to set filtering, wrapping,
 * and mipmaps.
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include<Windows.h>
#include<gl/GL.h>
#include<gl/GLU.h>

#include <string>
#include <memory>
#include <iostream>


 /**
  * @class Texture
  * @brief Encapsulates an OpenGL texture object with loading, binding, and parameter utilities.
  *
  * The Texture class provides functionality to create, manage, and configure
  * 2D, 3D, and CubeMap textures in OpenGL. It supports loading from files,
  * creating from existing OpenGL texture IDs, and configuring filtering and
  * wrapping modes. Textures are managed via factory methods returning shared
  * pointers to ensure proper resource handling.
  *
  * @note Copying is disabled to prevent multiple deletions of the same OpenGL texture.
  *       Moving is allowed to transfer ownership safely.
  *
  * @see FilterMode, WrapMode, Type
  */
class Texture {
public:

    /** @brief Texture filtering modes. */
    enum class FilterMode { Nearest, Linear, MipmapNearest, MipmapLinear };

    /** @brief Texture wrapping modes. */
    enum class WrapMode { Repeat, MirroredRepeat, ClampToEdge, ClampToBorder };

    /** @brief Type of texture. */
    enum class Type { Texture2D, Texture3D, CubeMap };

    /** @name Factory Methods */
    ///@{
    /**
     * @brief Creates a texture from a file path.
     * @param filepath Path to the texture file.
     * @param type Texture type (default is 2D).
     * @param flipVertical Whether to flip the image vertically.
     * @return Shared pointer to the created Texture.
     */
    static std::shared_ptr<Texture> create(const std::string& filepath, Type type = Type::Texture2D, bool flipVertical = true);

    /**
     * @brief Creates a texture from an existing OpenGL texture ID.
     * @param textureID OpenGL texture ID.
     * @param width Texture width.
     * @param height Texture height.
     * @param type Texture type (default is 2D).
     * @return Shared pointer to the created Texture.
     */
    static std::shared_ptr<Texture> create(GLuint textureID, const int& width, const int& height, Type type = Type::Texture2D);
    ///@}

    /** @brief Destructor that deletes the OpenGL texture. */
    ~Texture();

    //Default Constructor
    Texture();

    // Prevent copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Allow moving
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    /** @brief Binds the texture to a specified texture unit. */
    void bind(unsigned int unit = 0) const;

    /** @brief Unbinds the texture. */
    void unbind() const;

    /** @name Getters */
    ///@{
    GLuint getID() const { return texture_id; }  /**< Returns OpenGL texture ID. */
    int getWidth() const { return width; }      /**< Returns texture width. */
    int getHeight() const { return height; }    /**< Returns texture height. */
    int getChannels() const { return channels; }/**< Returns number of color channels. */
    GLenum getFormat() const { return format; } /**< Returns texture format. */
    Type getType() const { return type; }       /**< Returns texture type. */
    ///@}

    /** @name Texture Settings */
    ///@{
    void setFilterMode(FilterMode mode); /**< Sets the filtering mode. */
    void setWrapMode(WrapMode mode);     /**< Sets the wrapping mode. */
    void generateMipmaps();              /**< Generates mipmaps for the texture. */
    /// ///@}

    //new
    void newCreatingTexture(int Width, int Height, unsigned int* TextureId);
    //new
    void newBindTexture(const int unit, const int programId, const std::string& name, GLint TextureID);

    Texture(GLuint textureID, const int& width, const int& height, int channels, GLenum format, Type type);


    void  CreatingTexture(int Width, int Height, unsigned int* TextureId);



private:
    /** @brief Private constructors for factory usage. */
    Texture(const std::string& filepath, Type type, bool flipVertical);
    Texture(GLuint textureID, const int& width, const int& height, Type type);
    
   
    /** @brief Loads texture data from a file. */
    void loadTexture(const std::string& filepath, bool flipVertical = true);

    /** @brief Loads HDR texture data from a file. */
    void loadTextureHDR(const std::string& filepath);

    /** @brief Loads character texture data from a file. */
    void loadTextureCharactor(const std::string& filepath);

    /** @brief Sets default texture parameters (filtering, wrapping, etc.). */
    void setupTextureParameters();


    GLuint texture_id;  /**< OpenGL texture ID. */
    int width;         /**< Width of the texture. */
    int height;        /**< Height of the texture. */
    int channels;      /**< Number of color channels. */
    GLenum format;     /**< OpenGL format of the texture. */
    Type type;         /**< Type of the texture. */
    bool textureOn = false;
};
