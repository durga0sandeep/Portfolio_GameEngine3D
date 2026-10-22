/* *
Copyright(c) 2025 Digipen Institute of Technology
Author : Martin Chow
*/
#pragma once
#include "pch.h"
#include "Graphics/Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::shared_ptr<Texture> Texture::create(const std::string& filepath, Type type, bool flipVertical) {
    return std::shared_ptr<Texture>(new Texture(filepath, type, flipVertical));
}

std::shared_ptr<Texture> Texture::create(GLuint textureID, const int& width, const int& height, Type type)
{
    return std::shared_ptr<Texture>(new Texture(textureID, width, height, type));
}

//std::shared_ptr<Texture> Texture::create(const FT_Face& face, Type type, const unsigned char& c)
//{
//    return std::shared_ptr<Texture>(new Texture(face, c , type));
//}

Texture::Texture(const std::string& filepath, Type type, bool flipVertical)
    : texture_id(0), width(0), height(0), channels(0), format(GL_RGB), type(type) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    loadTexture(filepath, flipVertical);
    setupTextureParameters();
}

Texture::Texture(GLuint textureID, const int& width, const int& height, Type type)
    : texture_id(textureID), width(width), height(height), channels(0), format(GL_RGB), type(type) {

}

//new
void Texture::newCreatingTexture(int Width, int Height, unsigned int* TextureId)
{
    glGenTextures(1, TextureId);
    glBindTexture(GL_TEXTURE_2D, *TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//
void Texture::newBindTexture(const int unit, const int programId, const std::string& name, GLint TextureID)
{
    glActiveTexture((GLenum)((int)GL_TEXTURE0 + unit));
    glBindTexture(GL_TEXTURE_2D, TextureID);
    //commmenting for now
    //int loc = glGetUniformLocation(programId, name.c_str());
    //glUniform1i(loc, unit);
}

Texture::Texture(GLuint textureID, const int& width, const int& height, int channels, GLenum format, Type type)
    : texture_id(textureID), width(width), height(height), channels(channels), format(format), type(type)
{
}

void Texture::CreatingTexture(int Width, int Height, unsigned int* TextureId)
{
    glGenTextures(1, TextureId);
    glBindTexture(GL_TEXTURE_2D, *TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}


Texture::~Texture() {
    if (texture_id != 0) {
        glDeleteTextures(1, &texture_id);
    }
}

// Default constructor
Texture::Texture()
    : texture_id(0), width(0), height(0), channels(0), format(GL_RGB),
    type(Type::Texture2D), textureOn(false) {

    //

}



Texture::Texture(Texture&& other) noexcept
    : texture_id(other.texture_id), width(other.width), height(other.height),
    channels(other.channels), format(other.format), type(other.type) {
    other.texture_id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (texture_id != 0) {
            glDeleteTextures(1, &texture_id);
        }
        texture_id = other.texture_id;
        width = other.width;
        height = other.height;
        channels = other.channels;
        format = other.format;
        type = other.type;
        other.texture_id = 0;
    }
    return *this;
}

void Texture::loadTexture(const std::string& filepath, bool flipVertical) {
    stbi_set_flip_vertically_on_load(flipVertical);
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    //auto data = AssetManager::Instance().loadTextureImage(filepath.c_str(), &width, &height, &channels);
    if (!data) {
        throw std::runtime_error("Failed to load texture: " + filepath);
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

// if planning to use HDR skydome
void Texture::loadTextureHDR(const std::string& filepath) {
    stbi_set_flip_vertically_on_load(true);
    float* data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 0);

    if (!data) {
        throw std::runtime_error("Failed to load texture: " + filepath);
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, data);

    stbi_image_free(data);
}

// if planning to use HDR skydome
void Texture::loadTextureCharactor(const std::string& filepath) {
    stbi_set_flip_vertically_on_load(true);

    float* data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load texture: " + filepath);
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, data);

    stbi_image_free(data);
}


void Texture::setupTextureParameters() {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(unsigned int unit) const {
    //std::cout << "3 TEXTURE UNIT " << unit << std::endl;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setFilterMode(FilterMode mode) {

    //glBindTexture(GL_TEXTURE_2D, texture_id);
    switch (mode) {
    case FilterMode::Nearest:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case FilterMode::Linear:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case FilterMode::MipmapNearest:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);
        break;
    case FilterMode::MipmapLinear:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        break;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setWrapMode(WrapMode mode) {
    //glBindTexture(GL_TEXTURE_2D, texture_id);
    GLint glMode;
    switch (mode) {
    case WrapMode::Repeat:
        glMode = GL_REPEAT;
        break;
    case WrapMode::MirroredRepeat:
        glMode = GL_MIRRORED_REPEAT;
        break;
    case WrapMode::ClampToEdge:
        glMode = GL_CLAMP_TO_EDGE;
        break;
    case WrapMode::ClampToBorder:
        glMode = GL_CLAMP_TO_BORDER;
        break;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glMode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::generateMipmaps() {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}




