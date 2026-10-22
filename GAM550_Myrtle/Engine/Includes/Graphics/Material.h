/*!****************************************************************************
 * file  : Material.h
 * brief : Material - This class manages the Shader and associated properties (uniforms) for a game object.
 * It also manages textures and their binding to specific texture units.
 *
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <optional>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"

 /**
  * @class Material
  * @brief Represents a rendering material with shader, properties, and textures.
  *
  * The Material class encapsulates shader management, uniform properties,
  * and textures for rendering objects. It provides a flexible interface
  * for setting shader uniforms of various types and binding textures to
  * specific texture units.
  *
  * @note Call `apply()` before rendering to ensure the shader, properties,
  *       and textures are properly set for the object.
  */
class Material {
public:
    /**
     * @brief Constructs a Material with the given shader.
     * @param shader Shared pointer to the Shader object.
     */
    Material(std::shared_ptr<Shader> shader);

    /** @brief Virtual destructor. */
    virtual ~Material() = default;

    /** @name Shader Management */
    ///@{
    /** @brief Sets the shader used by this material. */
    void setShader(std::shared_ptr<Shader> shader);

    /** @brief Gets the shader used by this material. */
    std::shared_ptr<Shader> getShader() const;
    ///@}

    /** @name Property Setters (Uniforms) */
    ///@{
    void setProperty(const std::string& name, unsigned int value); /**< Set unsigned int property. */
    void setProperty(const std::string& name, int value);          /**< Set int property. */
    void setProperty(const std::string& name, float value);        /**< Set float property. */
    void setProperty(const std::string& name, bool value);        /**< Set bool property. */
    void setProperty(const std::string& name, const glm::vec2& value); /**< Set vec2 property. */
    void setProperty(const std::string& name, const glm::vec3& value); /**< Set vec3 property. */
    void setProperty(const std::string& name, const glm::vec4& value); /**< Set vec4 property. */
    void setProperty(const std::string& name, const glm::mat4& value); /**< Set mat4 property. */
    ///@}

    /**
     * @brief Sets a texture for the material and binds it to a texture unit.
     * @param name Name of the texture uniform in the shader.
     * @param texture Shared pointer to the Texture object.
     * @param unit Texture unit to bind the texture to.
     */
    void setTexture(const std::string& name, std::shared_ptr<Texture> texture, unsigned int unit);

    /**
     * @brief Applies the material by setting shader uniforms and binding textures.
     * Should be called before rendering the associated object.
     */
    virtual void apply() const;

    bool textureIsPresent();

    std::shared_ptr<Texture> getTexture(const std::string& name) const;


private:
    /** @brief Shader used by this material. */
    std::shared_ptr<Shader> shader;

    /** @brief Map storing property name to its value (various types supported). */
    std::unordered_map<std::string, std::variant<unsigned int, int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4>> properties;

    /**
     * @struct TextureInfo
     * @brief Stores texture and its assigned texture unit.
     */
    struct TextureInfo {
        std::shared_ptr<Texture> texture; /**< Pointer to the texture object. */
        unsigned int unit;                /**< Texture unit index. */
    };

    /** @brief Optional map storing textures for the material. */
    std::optional<std::unordered_map<std::string, TextureInfo>> texture_data;

    bool textureOn;

};
