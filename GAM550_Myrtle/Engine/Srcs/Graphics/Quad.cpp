#include "pch.h"
#include "Graphics/Quad.h"

Quad::Quad(std::string name) : Mesh(name)
{
    std::vector<float> vertices = {
        // Positions        // TexCoords
        -1.0f,  -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,   1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,   1.0f, 0.0f, 0.0f, 1.0f
    };
    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };
    setVertexData(
        {
            {GeometryBuffer::Attribute::Position, vertices, 3},
            {GeometryBuffer::Attribute::TexCoord, vertices, 2}
        },
        5 * sizeof(float), // stride: 2 pos + 2 texCoord
        indices
        );
}
