/* *
Copyright(c) 2025 Digipen Institute of Technology
Author : Martin Chow
*/
#include "pch.h"
#include "Graphics/Material.h"

Material::Material(std::shared_ptr<Shader> shader) : shader(shader), textureOn(false) {}

void Material::setShader(std::shared_ptr<Shader> newShader) {
    shader = newShader;
}

std::shared_ptr<Shader> Material::getShader() const {
    
    return shader;
}

void Material::setProperty(const std::string& name, unsigned int value) {
    properties[name] = value;
}

void Material::setProperty(const std::string& name, int value) {
    properties[name] = value;
}

void Material::setProperty(const std::string& name, float value) {
    properties[name] = value;
}

void Material::setProperty(const std::string& name, bool value)
{
    properties[name] = value;
}

void Material::setProperty(const std::string& name, const glm::vec3& value) {
    properties[name] = value;
}

void Material::setProperty(const std::string& name, const glm::vec4& value) {
    properties[name] = value;
}

void Material::setProperty(const std::string& name, const glm::vec2& value) {
    properties[name] = value;
}

void Material::setProperty(const std::string& name, const glm::mat4& value) {
    properties[name] = value;
}

void Material::setTexture(const std::string& name, std::shared_ptr<Texture> texture, unsigned int unit) {
    if (!texture_data) {
        texture_data = std::unordered_map<std::string, TextureInfo>();
    }
    (*texture_data)[name] = { texture, unit };
    textureOn = true;
    //std::cout << texture_data->size() << " ... " << std::endl;
}


std::shared_ptr<Texture> Material::getTexture(const std::string& name) const {
    
    if (!texture_data) {
        return nullptr; // No textures initialized
    }

    auto it = texture_data->find(name);
    if (it != texture_data->end()) {
        return it->second.texture; // Return the texture pointer
    }

    return nullptr; // Texture not found
}


void Material::apply() const {
    // Making sure shader exists
    if (!shader) {
        throw std::runtime_error("Shader not set for material");
    }

    shader->use();

    // Generic uniforms
    for (const auto& [name, value] : properties) {
        std::visit([this, &name](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, unsigned int>) {
                shader->setUInt(name, arg);
            }
            else if constexpr (std::is_same_v<T, int>) {
                shader->setInt(name, arg);
            }
            else if constexpr (std::is_same_v<T, float>) {
                shader->setFloat(name, arg);
            }
            else if constexpr (std::is_same_v<T, bool>) {
                shader->setBool(name, arg);
            }
            else if constexpr (std::is_same_v<T, glm::vec3>) {
                shader->setVec3(name, arg);
            }
            else if constexpr (std::is_same_v<T, glm::vec4>) {
                //std::cout << "vec4 name " << name << std::endl;
                shader->setVec4(name, arg);
            }
            else if constexpr (std::is_same_v<T, glm::mat4>) {
                shader->setMat4(name, arg);
            }
            else if constexpr (std::is_same_v<T, glm::vec2>) {
                //std::cout << "name " << name << std::endl;
                shader->setVec2(name, arg.x,arg.y);
            }
            }, value);
    }

    // Texture samplers
    if (texture_data) {
        for (const auto& [name, texInfo] : *texture_data) {
            if (texInfo.texture) {
                texInfo.texture->bind(texInfo.unit);
                shader->setInt(name, texInfo.unit);
            }
            else {
                // You might not want to use textures in your shader 
                // and/or your vertex data doesn't support it
                std::cerr << "Warning: Null texture for uniform " << name << std::endl;
            }
        }
    }

}

bool Material::textureIsPresent()
{
    return textureOn;
}

