/*!****************************************************************************
 * file  : GeometryBuffer.h
 * brief : GeometryBuffer - This class encapsulates a Vertex Array Object (VAO), Vertex Buffer Object (VBO),
 * and Element Buffer Object (EBO) to store vertex data and indices for rendering.
 * Each vertex attribute (position, normal, color, etc.) is stored separately for flexibility.
 *
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include "pch.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

 /**
  * @class GeometryBuffer
  * @brief Encapsulates vertex and index buffers for GPU rendering.
  *
  * The GeometryBuffer class manages vertex data, attributes, and indices
  * for OpenGL rendering. It provides functionality to create, update,
  * and query vertex and index data, as well as bind/unbind the associated
  * VAO for rendering.
  *
  * @note Use the static `create()` factory method to instantiate GeometryBuffer objects.
  * @note Copying is disabled to prevent accidental GPU resource duplication.
  */
class GeometryBuffer {
public:


    //GLint flagColor;

    /**
     * @enum Attribute
     * @brief Types of vertex attributes stored in the GeometryBuffer.
     */
    enum class Attribute {
        Position,     /**< Vertex position. */
        Normal,       /**< Vertex normal. */
        TexCoord,     /**< Texture coordinate. */
        Color,        /**< Vertex color. */
        Tangent,      /**< Tangent vector. */
        Bitangent,    /**< Bitangent vector. */
        JointIndices, /**< Joint indices for skeletal animation. */
        Weights       /**< Joint weights for skeletal animation. */
    };

    /**
     * @struct AttributeInfo
     * @brief Describes the layout of a vertex attribute.
     *
     * Used to provide flexibility when sending data to the GPU.
     */
    struct AttributeInfo {
        GLint       size;       /**< Number of components per vertex (e.g., 3 for vec3). */
        GLenum      type;       /**< Data type (e.g., GL_FLOAT). */
        GLboolean   normalized; /**< Whether the data should be normalized. */
        GLsizei     stride;     /**< Byte offset between consecutive vertices. */
    };


    static std::shared_ptr<GeometryBuffer> create(
        const std::unordered_map<Attribute, std::pair<std::vector<float>, AttributeInfo>>& attributeData,
        const std::vector<unsigned int>& indices,
        const std::string& name);

    
    ~GeometryBuffer();

    // Prevent copying
    GeometryBuffer(const GeometryBuffer&) = delete;
    GeometryBuffer& operator=(const GeometryBuffer&) = delete;

    // Allow moving
    GeometryBuffer(GeometryBuffer&&) noexcept;
    GeometryBuffer& operator=(GeometryBuffer&&) noexcept;

    /** @brief Binds the VAO for rendering. */
    void bind() const;

    /** @brief Unbinds the VAO. */
    void unbind() const;

    /** @brief Updates the data for a specific vertex attribute.
     *  @param attr The attribute to update.
     *  @param newData New vertex data for the attribute.
     */
    void updateVertexAttribute(Attribute attr, const std::vector<float>& newData);

    /** @brief Updates the index buffer.
     *  @param newIndices New index data.
     */
    void updateIndices(const std::vector<unsigned int>& newIndices);

    /** @brief Returns the number of vertices stored in the buffer. */
    GLsizei getVertexCount() const { return vertex_count; }

    /** @brief Returns the number of indices stored in the buffer. */
    GLsizei getIndexCount() const { return index_count; }

    /** @brief Returns the name of the GeometryBuffer. */
    const std::string& getName() const { return name; }

    /** @brief Checks whether the buffer has a specific attribute. */
    bool hasAttribute(Attribute attr) const;

    /** @brief Returns the index data. */
    auto getIndexData() const { return index_data; }

    /** @brief Returns the VAO handle. */
    GLuint getVAO() const { return vao; }

    /** @brief Returns the VBO handle. */
    GLuint getVBO() const { return vbo; }

    /** @brief Returns the EBO handle. */
    GLuint getEBO() const { return ebo; }

private:
    /** @brief Private constructor used by the factory method. */
    GeometryBuffer(const std::string& name);

    /** @brief Initializes GPU buffers with the given vertex and index data. */
    void initializeBuffers(
        const std::unordered_map<Attribute, std::pair<std::vector<float>, AttributeInfo>>& attributeData,
        const std::vector<unsigned int>& indices);

    /** @brief Cleans up GPU buffers. */
    void cleanupBuffers();

    GLuint vao;   /**< Vertex Array Object handle. */
    GLuint vbo;   /**< Vertex Buffer Object handle. */
    GLuint ebo;   /**< Element Buffer Object handle. */
    GLsizei vertex_count;   /**< Number of vertices in the buffer. */
    GLsizei index_count;    /**< Number of indices in the buffer. */
    std::unordered_map<Attribute, GLuint> attribute_offsets; /**< Attribute start offsets. */
    std::unordered_map<Attribute, AttributeInfo> attribute_infos; /**< Layout info for each attribute. */
    std::string name;      /**< Name for debugging and resource management. */
    std::vector<unsigned int> index_data; /**< Index data for drawing elements. */
};
