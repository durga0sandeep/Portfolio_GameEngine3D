/*!****************************************************************************
 * file  : Mesh.h
 * brief : Mesh - The Mesh class extends Geometry and manages vertex data, indices,
 * and attributes. It provides functions to set, update, and query
 * vertex and index data.
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include "Geometry.h"
#include <vector>
#include <unordered_map>
#include <string>

 /**
  * @class Mesh
  * @brief Represents a 3D mesh with vertex and index data, extending the Geometry interface.
  *
  * The Mesh class encapsulates vertex and index data for rendering 3D objects.
  * It provides methods to set and query vertex attributes, indices, and geometry buffers.
  * It also supports cloning and type identification as part of the Geometry interface.
  *
  * @note Mesh inherits from Geometry, so it implements all virtual functions required by the base class.
  */
class Mesh : public Geometry {
public:
    /** @brief Default constructor. */
    Mesh();

    /**
     * @brief Constructs a Mesh with a given name.
     * @param name Name of the mesh.
     */
    Mesh(const std::string& name);

    /** @brief Destructor. */
    ~Mesh() = default;

    /** @name Overridden Geometry Functions */
    ///@{
    std::shared_ptr<GeometryBuffer> getGeometryBuffer() const override; /**< Returns the internal GeometryBuffer. */
    std::string getType() const override;                                 /**< Returns the type of the geometry. */
    std::unique_ptr<Geometry> clone() const override;                     /**< Creates a copy of the mesh. */
    ///@}

    /** @name Mesh-Specific Functions */
    ///@{
    /**
     * @brief Sets the mesh's vertex data using an existing GeometryBuffer.
     * @param geometryBuffer Shared pointer to a GeometryBuffer.
     */
    void setVertexData(const std::shared_ptr<GeometryBuffer>& geometryBuffer);

    /**
     * @brief Sets vertex data and indices from raw VertexData and stride.
     * @param vertexData Vector of VertexData structs.
     * @param stride Byte offset between consecutive vertices.
     * @param indices Index buffer.
     */
    void setVertexData(
        const std::vector<VertexData>& vertexData,
        const GLsizei stride,
        const std::vector<unsigned int>& indices);

    /**
     * @brief Sets a specific vertex attribute.
     * @param attr Attribute type.
     * @param data Vector of floats for the attribute.
     * @param componentsPerVertex Number of components per vertex.
     * @param stride Byte stride between consecutive vertices.
     */
    void setAttributeData(
        GeometryBuffer::Attribute attr,
        const std::vector<float>& data,
        int componentsPerVertex,
        const GLsizei stride);

    /**
     * @brief Updates the index buffer.
     * @param newIndices Vector of new indices.
     */
    void setIndices(const std::vector<unsigned int>& newIndices);

    /** @brief Returns the number of vertices. */
    size_t getVertexCount() const;

    /** @brief Returns the number of indices. */
    size_t getIndexCount() const;

    /** @brief Checks if the mesh has a specific attribute. */
    bool hasAttribute(GeometryBuffer::Attribute attr) const;

    /** @brief Returns the mesh's name. */
    const std::string& getName() const;
    ///@}

    bool isGPUReady();

    void setReady();

    void setFail();

private:
    std::string name;                        /**< Name of the mesh. */
    std::vector<unsigned int> indices;       /**< Index buffer. */
    bool ready = false;
};
