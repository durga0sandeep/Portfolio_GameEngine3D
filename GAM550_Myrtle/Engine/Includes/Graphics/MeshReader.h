/*!****************************************************************************
 * file  : MeshReader.h
 * brief : MeshReader - MeshReader currently supports only OBJ files. It reads vertex positions, normals,
 * texture coordinates, and indices, then creates a single interleaved vertex array
 * suitable for uploading to GPU buffers.
 *
 * author: Durga Sandeep
 * Course: CS550
 * date  : 9/2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 /**
  * @class MeshReader
  * @brief Utility class for reading and processing mesh data from files.
  *
  * MeshReader provides static functions to load 3D mesh data from OBJ files
  * and generate interleaved vertex arrays suitable for OpenGL rendering.
  *
  * @note This class only provides static methods and should not be instantiated.
  */
class MeshReader
{
public:
    /**
     * @brief Reads a mesh from a file and outputs vertex and index arrays.
     *
     * This function updates the provided `vertex` and `indice` vectors by reference.
     * Internally, it calls `createInterleavedVertices` to generate a single interleaved
     * vertex array from separate positions, normals, and texture coordinates.
     *
     * @param filepath Path to the OBJ file.
     * @param vertex Output vector of floats storing interleaved vertex data.
     * @param indice Output vector of unsigned ints storing mesh indices.
     */
    static void readMesh(const std::string& filepath, std::vector<float>& vertex, std::vector<unsigned int>& indice);

private:
    /**
     * @brief Creates a single interleaved vertex array from separate attribute arrays.
     *
     * @param positions Vector of vertex positions.
     * @param normals Vector of vertex normals.
     * @param texCoords Vector of texture coordinates.
     * @param indices Output index buffer.
     * @param pindices Input indices for positions.
     * @param tindices Input indices for texture coordinates.
     * @param nindices Input indices for normals.
     * @param interleavedVertices Output vector storing interleaved vertex data.
     */
    static void createInterleavedVertices(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& normals,
        const std::vector<glm::vec2>& texCoords,
        std::vector<unsigned int>& indices,
        const std::vector<unsigned int>& pindices,
        const std::vector<unsigned int>& tindices,
        const std::vector<unsigned int>& nindices,
        std::vector<float>& interleavedVertices);
};




