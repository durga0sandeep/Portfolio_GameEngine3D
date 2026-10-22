/*!****************************************************************************
 * file  : Shader.h
 * brief : Shader - This class loads vertex, fragment, and other shader types, compiles them,
 * links them into a program, and provides convenient functions to set uniforms.
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Graphics/Fbo.h"

 /**
  * @class Shader
  * @brief Encapsulates an OpenGL shader program with utilities for compilation, linking, and uniform management.
  *
  * The Shader class provides functionality to load, compile, and link
  * vertex, fragment, and other shader types into a single shader program.
  * It also provides convenient methods to set uniform variables in the shader.
  *
  * @note Uniform locations are cached to minimize OpenGL queries.
  * @note Copying is disabled implicitly by default since the class manages OpenGL resources.
  */
class Shader {
public:
    /**
     * @brief Constructs a Shader from a single string containing shader paths.
     * @param shaderPaths Paths to the shader files (vertex, fragment, etc.).
     */
   
    Shader();

    Shader(const std::string& shaderPaths);

    /** @brief Destructor that cleans up shader resources. */
    ~Shader();

    /** @brief Activates the shader program. */
    void use() const;

    /** @brief Deactivates the shader program. */
    void unuse() const;

    /** @name Uniform Setters */
    ///@{
    void setUInt(const std::string& name, unsigned int value) const;             /**< Set unsigned int uniform. */
    void setInt(const std::string& name, int value) const;                       /**< Set int uniform. */
    void set2Int(const std::string& name, int v0, int v1) const;                 /**< Set ivec2 uniform. */
    void setFloat(const std::string& name, float value) const;                   /**< Set float uniform. */
    void setBool(const std::string& name, bool value) const;
    void setVec2(const std::string& name, float x, float y) const;               /**< Set vec2 uniform. */
    void setVec3(const std::string& name, const glm::vec3& value) const;         /**< Set vec3 uniform. */
    void setVec4(const std::string& name, const glm::vec4& value) const;         /**< Set vec4 uniform. */
    void setVec4(const std::string& name, float x, float y, float z, float w) const; /**< Set vec4 uniform using components. */
    void setMat4(const std::string& name, const glm::mat4& value) const;         /**< Set mat4 uniform. */
    /// ///@}

    /** @brief Returns the OpenGL program ID. */
    unsigned int getID() const { return program_id; }

    void SetFlagColor(int i);

    //void SetFbo
    void setDepthTestOn();
    void setDepthTestOff();

    void setFbo(std::shared_ptr<FBO>  fbo);
    std::shared_ptr<FBO> getFbo();

    /*
    void activateFbo(int w, int h);
    void createFbo_Multi();
    void bindFbo();
    void unbindFbo();
    int getFboDimension();
    //Deferred Shading
    FBO* gFbo;

    */


    // Getter for BlurSMTextureId
    unsigned int getBlurSMTextureId() const
    {
        return BlurSMTextureId;
    }

    // Getter for FinalSMTextureId
    unsigned int getFinalSMTextureId() const
    {
        return FinalSMTextureId;
    }

    // Setter for BlurSMTextureId
    void setBlurSMTextureId(unsigned int textureId)
    {
        BlurSMTextureId = textureId;
    }

    // Setter for FinalSMTextureId
    void setFinalSMTextureId(unsigned int textureId)
    {
        FinalSMTextureId = textureId;
    }




    void BindImageTexture(int unit, unsigned int Textureid, const GLenum access, int ShaderId, const char* name);

    void DispatchComputerShader(int Width, int Height, int group_x, int group_y);






    unsigned int BlurSMTextureId, FinalSMTextureId;

private:
    unsigned int program_id; /**< OpenGL shader program ID. */
    //new

    mutable std::unordered_map<std::string, int> uniform_location_cache; /**< Cache of uniform locations. */
    std::unordered_map<GLenum, unsigned int> shader_ids; /**< Map of individual shader IDs by type. */

    /** @brief Compiles a shader from source code. */
    unsigned int loadShader(const std::string& shaderCode, GLenum shaderType);

    /** @brief Links all compiled shaders into a shader program. */
    void linkShaders();

    /** @brief Retrieves the location of a uniform in the shader program. */
    int getUniformLocation(const std::string& name) const;

    /** @brief Determines shader type from file extension. */
    GLenum getShaderType(const std::string& filePath);

    /** @brief Reads the shader source code from a file. */
    std::string readShaderFile(const std::string& filePath);

    /** @brief Converts a string to lowercase. */
    std::string toLowercase(const std::string& str);

    GLint flagColor;


    //
    //
    bool fboNeeded;

    //Deferred Shading
    std::shared_ptr<FBO> gFbo;
    int GBufferMap_Width, GBufferMap_Height;



};
