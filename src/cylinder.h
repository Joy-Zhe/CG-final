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


class Cylinder {
public:
    Cylinder(std::string texture_path);

    ~Cylinder();

    void draw(const glm::mat4& projection, const glm::mat4& view, std::shared_ptr<DirectionalLight> _light, glm::vec3 position, std::shared_ptr<AmbientLight> _ambientLight, std::shared_ptr<SpotLight> _spotLight, std::shared_ptr<Material> _material);

    Transform transform;

private:
    GLuint _vao = 0;

    GLuint _vbo = 0;

    GLuint _ebo = 0;

    // 顶点数组
    unsigned int indices[660];
    vertex_tri vertices[143];

    //std::vector<unsigned int> indices;
    //std::vector<vertex_tri> vertices;

    float height = 8.0;
    float radius = 1.0f; // 底面半径

    std::unique_ptr<GLSLProgram> _shader;
    GLuint _textureID;
    std::unique_ptr<SimpleMaterial> simpleMaterial;
    void setdata();

};