/* *
Copyright(c) 2025 Digipen Institute of Technology
Author : Martin Chow
*/
#include "pch.h"
#include "Graphics/Geometry.h"

/**
 * @brief Prepares attribute data for the GeometryBuffer.
 *
 * Converts a list of VertexData into a format suitable for the GeometryBuffer,
 * including setting up the stride and attribute info.
 *
 * @param attributeMap Map that stores the attribute data and associated info.
 * @param vertexData Vector of VertexData to process.
 * @param stride The stride (byte offset) between consecutive vertices.
 */
void Geometry::prepareAttributeData(
    std::unordered_map<
    GeometryBuffer::Attribute,
    std::pair<std::vector<float>, GeometryBuffer::AttributeInfo>>&attributeMap,
    const std::vector<VertexData>& vertexData,
    const GLsizei stride)
{
    for (const auto& vda : vertexData) {
        attributeMap[vda.attribute] = {
            vda.data,
            {vda.components_per_vertex, GL_FLOAT, GL_FALSE, stride}
        };
    }
}
