#include <algorithm>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <glad/gl.h>
#include "newmodel.h"
#include<string>
#include<sstream>
#include<fstream>

void exportModelToObj(const std::string& filepath, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Failed to open file " + filepath);
    }

    std::unordered_map<glm::vec2, uint32_t> texCoordIndices;
    std::unordered_map<glm::vec3, uint32_t> normalIndices;

    // Build texture coordinates and normal indices maps
    for (const Vertex& vertex : vertices) {
        if (texCoordIndices.count(vertex.texCoord) == 0) {
            texCoordIndices[vertex.texCoord] = static_cast<uint32_t>(texCoordIndices.size() + 1);
        }
        if (normalIndices.count(vertex.normal) == 0) {
            normalIndices[vertex.normal] = static_cast<uint32_t>(normalIndices.size() + 1);
        }
    }

    // Write vertices
    for (const Vertex& vertex : vertices) {
        // Write vertex position
        file << "v " << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << std::endl;
    }

    // Write texture coordinates
    for (const auto& texCoord : texCoordIndices) {
        file << "vt " << texCoord.first.x << " " << texCoord.first.y << std::endl;
    }

    // Write normals
    for (const auto& normal : normalIndices) {
        file << "vn " << normal.first.x << " " << normal.first.y << " " << normal.first.z << std::endl;
    }

    // Write indices (faces)
    for (size_t i = 0; i < indices.size(); i += 3) {
        file << "f ";
        for (size_t j = 0; j < 3; ++j) {
            uint32_t index = indices[i + j] + 1;
            const Vertex& vertex = vertices[index - 1];

            // Get the texture coordinate and normal indices for the vertex
            uint32_t texCoordIndex = texCoordIndices[vertex.texCoord];
            uint32_t normalIndex = normalIndices[vertex.normal];

            // Write face indices
            file << index << "/" << texCoordIndex << "/" << normalIndex << " ";
        }
        file << std::endl;
    }

    file.close();
}
