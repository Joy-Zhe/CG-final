#include <algorithm>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <glad/gl.h>
#include "newmodel.h"
#include<string>
#include<sstream>
#include<fstream>

extern void  exportModelToObj(const std::string& filepath, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
//newmodel is a class which can produce a model when users provide a file path

newmodel::newmodel(const std::string& filepath)
{

    //key
    //fill in std::vector<Vertex> _vertices;
    //fill in std::vector<uint32_t> _indices;
    
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << filepath << std::endl;
        return;
    }

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<Vertex, uint32_t> uniqueVertices;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;


    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 position;
            ss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "vt") {
            glm::vec2 texCoord;
            ss >> texCoord.x >> texCoord.y;
            texCoord.y = -texCoord.y;
            texCoords.push_back(texCoord);
        }
        else if (prefix == "f")
        {
            std::vector<std::string> faceTokens;
            std::string token;
            while (ss >> token)
            {
                faceTokens.push_back(token);
            }

            if (faceTokens.size() < 3)
            {
                throw std::runtime_error("Invalid face data in file: " + filepath);
            }

            else if(faceTokens.size()==3)
                for (size_t i = 1; i <= faceTokens.size(); i++)
                {
                    std::istringstream faceIss(faceTokens[i - 1]);
                    int posIndex, texCoordIndex, normalIndex;
                    char slash;

                    faceIss >> posIndex >> slash >> texCoordIndex >> slash >> normalIndex;

                    Vertex v(positions[posIndex - 1], normals[normalIndex - 1], texCoords[texCoordIndex - 1]);

                    if (uniqueVertices.count(v) == 0) {
                        uniqueVertices[v] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(v);
                    }

                    indices.push_back(uniqueVertices[v]);
                }

            else
            {
                uint32_t index[4];
                for (size_t i = 1; i <= faceTokens.size(); i++)
                {
                    std::istringstream faceIss(faceTokens[i - 1]);
                    int posIndex, texCoordIndex, normalIndex;
                    char slash;

                    faceIss >> posIndex >> slash >> texCoordIndex >> slash >> normalIndex;

                    Vertex v(positions[posIndex - 1], normals[normalIndex - 1], texCoords[texCoordIndex - 1]);

                    if (uniqueVertices.count(v) == 0) {
                        uniqueVertices[v] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(v);
                    }
                    index[i - 1] = uniqueVertices[v];
                }

                // Add triangle 1 indices
                indices.push_back(index[0]);
                indices.push_back(index[1]);
                indices.push_back(index[2]);
                // Add triangle 2 indices
                indices.push_back(index[0]);
                indices.push_back(index[2]);
                indices.push_back(index[3]);
            }
        }
        
    }
     

    std::cout << "Loaded " << vertices.size() << " vertices and " << indices.size() / 3 << " triangles" << std::endl;
    std::cout<< indices.max_size()<<std::endl;

    _vertices = vertices;
    _indices = indices;

    computeBoundingBox();

    initGLResources();

    initBoxGLResources();

    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      cleanup();
       throw std::runtime_error("OpenGL Error: " + std::to_string(error));
    }
    
    //exportModelToObj("export.txt", _vertices, _indices);

}


newmodel:: ~newmodel()
{
    cleanup();
}

GLuint newmodel:: getVao() const
{
    return _vao;
}


GLuint newmodel::getBoundingBoxVao() const
{
	return _boxVao;
}

size_t newmodel::getVertexCount() const
{
    	return _vertices.size();
}

size_t newmodel::getFaceCount() const
{
		return _indices.size() / 3;
}

BoundingBox newmodel:: getBoundingBox() const
{
        return _boundingBox;
}

void newmodel::draw() const
{
    glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void newmodel::drawBoundingBox() const
{
	glBindVertexArray(_boxVao);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void newmodel::computeBoundingBox()
{
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = -std::numeric_limits<float>::max();
    float maxY = -std::numeric_limits<float>::max();
    float maxZ = -std::numeric_limits<float>::max();

    for (const auto& v : _vertices) {
        minX = std::min(v.position.x, minX);
        minY = std::min(v.position.y, minY);
        minZ = std::min(v.position.z, minZ);
        maxX = std::max(v.position.x, maxX);
        maxY = std::max(v.position.y, maxY);
        maxZ = std::max(v.position.z, maxZ);
    }

    _boundingBox.min = glm::vec3(minX, minY, minZ);
    _boundingBox.max = glm::vec3(maxX, maxY, maxZ);

}

void newmodel::initGLResources()
{
    std::vector<glm::vec3> boxVertices = {
       glm::vec3(_boundingBox.min.x, _boundingBox.min.y, _boundingBox.min.z),
       glm::vec3(_boundingBox.max.x, _boundingBox.min.y, _boundingBox.min.z),
       glm::vec3(_boundingBox.min.x, _boundingBox.max.y, _boundingBox.min.z),
       glm::vec3(_boundingBox.max.x, _boundingBox.max.y, _boundingBox.min.z),
       glm::vec3(_boundingBox.min.x, _boundingBox.min.y, _boundingBox.max.z),
       glm::vec3(_boundingBox.max.x, _boundingBox.min.y, _boundingBox.max.z),
       glm::vec3(_boundingBox.min.x, _boundingBox.max.y, _boundingBox.max.z),
       glm::vec3(_boundingBox.max.x, _boundingBox.max.y, _boundingBox.max.z),
    };

    std::vector<uint32_t> boxIndices = { 0, 1, 0, 2, 0, 4, 3, 1, 3, 2, 3, 7,
                                        5, 4, 5, 1, 5, 7, 6, 4, 6, 7, 6, 2 };

    glGenVertexArrays(1, &_boxVao);
    glGenBuffers(1, &_boxVbo);
    glGenBuffers(1, &_boxEbo);

    glBindVertexArray(_boxVao);
    glBindBuffer(GL_ARRAY_BUFFER, _boxVbo);
    glBufferData(
        GL_ARRAY_BUFFER, boxVertices.size() * sizeof(glm::vec3), boxVertices.data(),
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _boxEbo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, boxIndices.size() * sizeof(uint32_t), boxIndices.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void newmodel::initBoxGLResources() 
{
    glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(
		GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(),
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint32_t), _indices.data(),
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(
		2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void newmodel::cleanup()
{
    if (_boxEbo) {
        glDeleteBuffers(1, &_boxEbo);
        _boxEbo = 0;
    }

    if (_boxVbo) {
        glDeleteBuffers(1, &_boxVbo);
        _boxVbo = 0;
    }

    if (_boxVao) {
        glDeleteVertexArrays(1, &_boxVao);
        _boxVao = 0;
    }

    if (_ebo != 0) {
        glDeleteBuffers(1, &_ebo);
        _ebo = 0;
    }

    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }

    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
}