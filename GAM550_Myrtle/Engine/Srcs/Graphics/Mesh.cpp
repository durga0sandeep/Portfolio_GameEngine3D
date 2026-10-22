/* *
Copyright(c) 2025 Digipen Institute of Technology
Author : Martin Chow
*/
#include "pch.h"
#include "Graphics/Mesh.h"


Mesh::Mesh() {}

Mesh::Mesh(const std::string& name) : name(name) {}

void Mesh::setVertexData(
    const std::shared_ptr<GeometryBuffer>& geometryBuffer) {
    this->geometryBuffer = geometryBuffer;
    this->indices = geometryBuffer->getIndexData();
}

void Mesh::setVertexData(
    const std::vector<VertexData>& vertexData,
    const GLsizei stride,
    const std::vector<unsigned int>& indices) {
    std::unordered_map<GeometryBuffer::Attribute, std::pair<std::vector<float>, GeometryBuffer::AttributeInfo>> attributeMap;
    prepareAttributeData(attributeMap, vertexData, stride);
    geometryBuffer = GeometryBuffer::create(attributeMap, indices, name);
    this->indices = indices;
}

// mostly to update an attribute's array of data
// but can be used to initialize, too
void Mesh::setAttributeData(
    GeometryBuffer::Attribute attr,
    const std::vector<float>& data,
    int componentsPerVertex,
    const GLsizei stride) {
    VertexData vertexData{ attr, data, componentsPerVertex };
    std::unordered_map<GeometryBuffer::Attribute, std::pair<std::vector<float>, GeometryBuffer::AttributeInfo>> attributeMap;
    prepareAttributeData(attributeMap, { vertexData }, stride);

    if (geometryBuffer)
        geometryBuffer->updateVertexAttribute(attr, data);

    else
        geometryBuffer = GeometryBuffer::create(attributeMap, indices, name);

}

void Mesh::setIndices(const std::vector<unsigned int>& newIndices) {
    indices = newIndices;
    if (geometryBuffer) {
        geometryBuffer->updateIndices(newIndices);
    }
}

std::shared_ptr<GeometryBuffer> Mesh::getGeometryBuffer() const {
    return geometryBuffer;
}

std::string Mesh::getType() const {
    return "Mesh";
}

std::unique_ptr<Geometry> Mesh::clone() const {
    return std::make_unique<Mesh>(*this);
}

size_t Mesh::getVertexCount() const {
    return geometryBuffer ? geometryBuffer->getVertexCount() : 0;
}

size_t Mesh::getIndexCount() const {
    return geometryBuffer ? geometryBuffer->getIndexCount() : 0;
}

bool Mesh::hasAttribute(GeometryBuffer::Attribute attr) const {
    return geometryBuffer && geometryBuffer->hasAttribute(attr);
}

const std::string& Mesh::getName() const {
    return name;
}

bool Mesh::isGPUReady()
{
    return ready;
}

void Mesh::setReady()
{
    ready = true;
}

void Mesh::setFail()
{
    ready = false;
}
