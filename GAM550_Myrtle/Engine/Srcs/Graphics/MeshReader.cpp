/* *
Copyright(c) 2025 Digipen Institute of Technology
Author : Martin Chow
*/
#include "pch.h"
#include "Graphics/MeshReader.h"

// this is the opengl
//https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

void MeshReader::readMesh(const std::string& filePath, std::vector<float>& vertex, std::vector<unsigned int>& indices) {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> pindices;
    std::vector<unsigned int> tindices;
    std::vector<unsigned int> nindices;
    int count = 0;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // using istring stream instead of fscanf, this is C++
        std::istringstream ss(line);
        std::string prefix;

        //get the prefix
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 position;
            ss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (prefix == "vt") {
            glm::vec2 texCoord;
            ss >> texCoord[0] >> texCoord[1];
            texCoords.push_back(texCoord);
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "f") {
            //unsigned int vertexIndex[3], texCoordIndex[3], normalIndex[3];
            //char slash;
            std::string vertex;

            for (int i = 0; i < 3; ++i) {
                //ss >> vertexIndex[i] >> slash >> texCoordIndex[i] >> slash >> normalIndex[i];
                //pindices.push_back(vertexIndex[i] - 1); // OBJ indices start at 1
                //tindices.push_back(texCoordIndex[i] - 1);
                //nindices.push_back(normalIndex[i] - 1);
                //indices.push_back(count);
                //count++;

                //read the next line
                ss >> vertex;

                size_t slash1 = vertex.find('/');
                size_t slash2 = vertex.find('/', slash1 + 1);

                // Extract vertex index (always present)
                try {
                    unsigned int vertexIndex = std::stoi(vertex.substr(0, slash1));
                    pindices.push_back(vertexIndex - 1); // OBJ indices start at 1
                }
                catch (const std::exception& e) {
                    std::cerr << "Error parsing vertex index in file " << filePath << ": " << vertex << std::endl << e.what() << std::endl;
                    assert(false && "Failed to parse vertex index"); // This will trigger the assertion
                }
                // Extract texture coordinate index if available
                if (slash1 != std::string::npos && slash2 != slash1 + 1) {
                    // There's a texture index (not v//n format)
                    unsigned int texCoordIndex = std::stoi(vertex.substr(slash1 + 1, slash2 - slash1 - 1));
                    tindices.push_back(texCoordIndex - 1);
                }
                else {
                    // No texture index, use default
                    tindices.push_back(0); // Or another default value
                }

                // Extract normal index if available
                if (slash2 != std::string::npos) {
                    // There's a normal index
                    unsigned int normalIndex = std::stoi(vertex.substr(slash2 + 1));
                    nindices.push_back(normalIndex - 1);
                }
                else {
                    // No normal index, use default
                    nindices.push_back(0); // Or another default value
                }

                indices.push_back(count);
                count++;

            }

        }
    }

    file.close();
    createInterleavedVertices(positions, normals, texCoords, indices, pindices, tindices, nindices, vertex);
}

/**
 * @brief createInterleavedVertices
 *
 * Converting the 3 vector of normals with indices
 * indices is something like this: v/t/n v/t/n v/t/n on each face of traiangle
 * where v ,t ,n is the index of each vector
 *
 */
void MeshReader::createInterleavedVertices(const std::vector<glm::vec3>& positions
    , const std::vector<glm::vec3>& normals
    , const std::vector<glm::vec2>& texCoords
    , std::vector<unsigned int>& indices
    , const std::vector<unsigned int>& pindices
    , const std::vector<unsigned int>& tindices
    , const std::vector<unsigned int>& nindices
    , std::vector<float>& interleavedVertices) {

    // indices.size is number of v,t,n is use
    // interleavedVertices.reserve(indices.size()/3 * (8)); // 3 for position, 3 for normal, 2 for texCoord
    // std::cout << indices.size() / 3 * (8) << std::endl;

    // use unsigned int just to make sure we have enough index, not sure how complex it need
    for (unsigned int i = 0; i < indices.size(); i++) {
        // indices is something like this: v/t/n v/t/n v/t/n on each face of traiangle
        // I think this is right, if this is wrong when you load your object, please tell me
        glm::vec3 position = positions[pindices[i]];
        glm::vec2 texCoord = texCoords[tindices[i]];
        glm::vec3 normal = normals[nindices[i]];

        // Position
        interleavedVertices.push_back(position.x);
        interleavedVertices.push_back(position.y);
        interleavedVertices.push_back(position.z);

        // Normal
        interleavedVertices.push_back(normal.x);
        interleavedVertices.push_back(normal.y);
        interleavedVertices.push_back(normal.z);

        // Texture Coordinate
        interleavedVertices.push_back(texCoord[0]);
        interleavedVertices.push_back(texCoord[1]);

    }
    //std::cout << interleavedVertices.size() << std::endl;
}

