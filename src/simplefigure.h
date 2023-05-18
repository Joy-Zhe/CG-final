#pragma once

#include "base/application.h"
#include "base/glsl_program.h"
#include "stb_image.h"
#include "base/texture2d.h"
#include <memory>


struct SimpleMaterial {
    std::shared_ptr<Texture2D> mapKd;
};
struct vertex_tri {
    glm::vec3 position;
    glm::vec3 color;
};

class Triangle {
public:
    Triangle();

    ~Triangle();

    void draw(const glm::mat4& projection, const glm::mat4& view);
private:
    GLuint _vao = 0;

    GLuint _vbo = 0;

    GLuint _ebo = 0;

    vertex_tri _vertices[48] = {
        //front
        { glm::vec3(-0.8f, -0.9f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.8f, -0.7f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.7f, -0.7f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.7f, -0.9f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

        //back
        { glm::vec3(-0.8f, -0.9f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.8f, -0.7f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.7f, -0.7f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.7f, -0.9f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},

        //left
        { glm::vec3(-0.8f, -0.9f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.8f, -0.7f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.8f,-0.7f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.8f, -0.9f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

        //right
        { glm::vec3(-0.7f, -0.9f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.7f, -0.7f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.7f,-0.7f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.7f, -0.9f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

        //up
        { glm::vec3(-0.8f, -0.7f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.8f, -0.7f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.7f, -0.7f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.7f, -0.7f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

        //down
        { glm::vec3(-0.8f, -0.9f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.8f, -0.9f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.7f, -0.9f, -0.2f), glm::vec3(0.0f, 0.0f, 0.0f)},
        { glm::vec3(-0.7f, -0.9f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)}
    };

    unsigned int _indices[36] = {
        0, 1, 3,
        1, 2, 3,

        4, 5, 7,
        5, 6, 7,

        8, 9, 11,
        9, 10, 11,

        12, 13, 15,
        13, 14, 15,

        16, 17, 19,
        17, 18, 19,

        20, 21, 23,
        21, 22, 23,
    };

    std::unique_ptr<GLSLProgram> _shader;
    GLuint _textureID;

    std::unique_ptr<SimpleMaterial> simpleMaterial;

};
