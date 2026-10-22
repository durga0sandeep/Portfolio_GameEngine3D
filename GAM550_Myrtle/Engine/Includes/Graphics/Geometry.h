/*!****************************************************************************
 * file  : Geometry.h
 * brief : Geometry - Each VertexData instance holds data for a specific attribute of the geometry,
 * such as position, normal, color, or texture coordinates.
 *
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include "GeometryBuffer.h"
#include <memory>
#include <string>
#include <vector>


struct VertexData {
    GeometryBuffer::Attribute attribute; /**< Type of the vertex attribute (position, normal, etc.). */
    std::vector<float> data;             /**< Raw float data for the attribute. */
    int components_per_vertex;             /**< Number of components per vertex (e.g., 3 for vec3). */
};

/**
 * @class Geometry
 * @brief Base class for geometric objects storing a list of vertex attributes.
 *
 * This class manages vertex data and provides an interface for:
 * - Accessing the underlying GeometryBuffer
 * - Getting the geometry type
 * - Cloning the geometry
 *
 * Each vertex can contain multiple attributes such as:
 * - Position
 * - Normal
 * - TexCoord
 * - Color
 * - Tangent
 * - Bitangent
 *
 * You do not need to use all attributes in every geometry.
 */
class Geometry {
public:
    /** @brief Virtual destructor. */
    virtual ~Geometry() = default;

    /**
     * @brief Returns the associated GeometryBuffer.
     * @return Shared pointer to the GeometryBuffer.
     */
    virtual std::shared_ptr<GeometryBuffer> getGeometryBuffer() const = 0;

    /**
     * @brief Returns the type of the geometry as a string.
     * @return std::string Type name of the geometry.
     */
    virtual std::string getType() const = 0;

    /**
     * @brief Clones the geometry object.
     * @return Unique pointer to a new Geometry object that is a copy of this one.
     */
    virtual std::unique_ptr<Geometry> clone() const = 0;

protected:
    /** @brief Shared pointer to the internal GeometryBuffer. */
    std::shared_ptr<GeometryBuffer> geometryBuffer;


    void prepareAttributeData(
        std::unordered_map<
        GeometryBuffer::Attribute,
        std::pair<std::vector<float>, GeometryBuffer::AttributeInfo>>&attributeMap,
        const std::vector<VertexData>& vertexData,
        const GLsizei stride);
};
