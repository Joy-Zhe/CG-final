#pragma once

#include <vector>
#include "base/application.h"
#include "base/glsl_program.h"
#include "stb_image.h"
#include "base/texture2d.h"
#include <memory>
#include"base/transform.h"
#include"base/light.h"
#include"cube.h"


class Pyramid {
public:
    Pyramid(std::string texture_path);

    ~Pyramid();

    void draw(const glm::mat4& projection, const glm::mat4& view, std::shared_ptr<DirectionalLight> _light);

    float radius = 1.0f; // 球体半径
    Transform transform;

private:
    GLuint _vao = 0;

    GLuint _vbo = 0;

    GLuint _ebo = 0;

    // 顶点数组
    unsigned int indices[18];
    vertex_tri vertices[7];

    float height = 8.0;

    std::unique_ptr<GLSLProgram> _shader;
    GLuint _textureID;
    std::unique_ptr<SimpleMaterial> simpleMaterial;
    void setdata();

};