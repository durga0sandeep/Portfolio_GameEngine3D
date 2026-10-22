#include "pch.h"
#include "Graphics/SphereClass.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

SphereClass::SphereClass(std::string name, unsigned int segments, float radius)
    : Mesh(name)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float PI = 3.14f;

    for (unsigned int y = 0; y <= segments; ++y)
    {
        for (unsigned int x = 0; x <= segments; ++x)
        {
            float xSegment = (float)x / (float)segments;
            float ySegment = (float)y / (float)segments;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            // Position
            vertices.push_back(xPos * radius);
            vertices.push_back(yPos * radius);
            vertices.push_back(zPos * radius);

            // TexCoord
            vertices.push_back(xSegment);
            vertices.push_back(ySegment);
        }
    }

    bool odd = false;
    for (unsigned int y = 0; y < segments; ++y)
    {
        for (unsigned int x = 0; x < segments; ++x)
        {
            unsigned int i0 = y * (segments + 1) + x;
            unsigned int i1 = (y + 1) * (segments + 1) + x;
            unsigned int i2 = (y + 1) * (segments + 1) + (x + 1);
            unsigned int i3 = y * (segments + 1) + (x + 1);

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i2);
            indices.push_back(i3);
            indices.push_back(i0);
        }
    }

    // Each vertex has:
    // 3 floats position + 2 floats texcoord = 5 floats per vertex
    setVertexData(
        {
            { GeometryBuffer::Attribute::Position, vertices, 3 },
            { GeometryBuffer::Attribute::TexCoord, vertices, 2 }
        },
        5 * sizeof(float),
        indices
    );
}
