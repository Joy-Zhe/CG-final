#pragma once

#include "base/application.h"
#include "base/glsl_program.h"
#include "stb_image.h"
#include "base/texture2d.h"
#include <memory>
#include"base/transform.h"
#include"base/light.h"

struct SimpleMaterial {
    std::shared_ptr<Texture2D> mapKd;
};

struct vertex_tri {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
};

struct Material {
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    float ns;
};

class Cube {
public:
    Cube(std::string texture_path);

    ~Cube();

    void draw(const glm::mat4& projection, const glm::mat4& view, std::shared_ptr<DirectionalLight> _light, glm::vec3 position, std::shared_ptr<AmbientLight> _ambientLight, std::shared_ptr<SpotLight> _spotLight, std::shared_ptr<Material> _material);

    Transform transform;
private:
    GLuint _vao = 0;

    GLuint _vbo = 0;

    GLuint _ebo = 0;
    

    // 顶点数组
    vertex_tri _vertices[8] = {
        // Front face
        { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.25f, 0.25f) },
        { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.75f, 0.25f) },
        { glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.75f, 0.75f) },
        { glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.25f, 0.75f) },
        // Back face
        { glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) }
    };


    // 索引数组
    unsigned int _indices[36] = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Back face
        4, 5, 6,
        6, 7, 4,
        // Left face
        7, 3, 0,
        0, 4, 7,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Top face
        7, 6, 2,
        2, 3, 7,
        // Buttom face
        4, 5, 1,
        4, 0, 1
    };

    std::unique_ptr<GLSLProgram> _shader;
    GLuint _textureID;

    std::unique_ptr<SimpleMaterial> simpleMaterial;

};